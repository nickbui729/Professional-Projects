import logging
import os
from subprocess import PIPE, run
import tempfile
import shutil
import ntpath
from django.conf import settings
from django.http import HttpResponseRedirect
from django.shortcuts import render, redirect
# from django.urls import reverse
from django.views.generic import (DetailView,
                                  CreateView,
                                  UpdateView)
from django.contrib.auth.decorators import login_required
from django.template.loader import get_template
from django.conf import settings

from django_tables2 import RequestConfig
from django.core.mail import EmailMessage
from account.models import Account
from account.models import Profile

DEFAULT_INTERPRETER = 'lualatex'


# import the Award class from models file so calls can be made to/from database
from .models import Award
from .tables import AwardTable
from .forms import AwardForm


logger = logging.getLogger(__name__)


@login_required
def home(request):
    logger.info("test view")
    table = AwardTable(Award.objects.filter(submitter=request.user))
    RequestConfig(request).configure(table)

    return render(request, 'awards/home.html', {'table': table})

@login_required
def send(request, *args, **kwargs):
    context = {}
    sendpdf(kwargs['pk'])
    response = redirect('awards-home')
    #return render(request, 'awards/test.html', context)
    return response


@login_required
def delete(request, *args, **kwargs):
    Award.objects.filter(pk=kwargs['pk']).delete()
    response = redirect('awards-home')
    table = AwardTable(Award.objects.filter(submitter=request.user))
    RequestConfig(request).configure(table)
    return response


def sendpdf(awardId):
    award = Award.objects.get(pk=awardId)
    awardsubmitter = Account.objects.get(username=award.submitter)
    awarduser = Profile.objects.get(user=awardsubmitter)
    template_name = 'award_cert.tex'
    # print(award)
    context = {'awardid': awardId,
               'recipientfname': award.recipient_fname,
               'recipientlname': award.recipient_lname,
               'category': award.category,
               'submitterfname': awardsubmitter.first_name,
               'submitterlname': awardsubmitter.last_name,
               'awarddate': award.date_granted.strftime("%B %d, %Y")}
    print(awarduser.image.url.lstrip('/'))
    # print('test')
    # print(context)
    template = get_template(template_name, using='tex')
    source = template.render(context)
    with tempfile.TemporaryDirectory() as tempdir:
        # copy the signature file from the media directory to the current temp directory
        shutil.copy2(os.path.join('C:\\Users\Amy\Documents\CS467\ERP1\\backend\\',awarduser.image.url.lstrip('/')),
                     tempdir)
        # rename the file to signature.jpg because that is the filename the Latex template is looking for
        os.rename(os.path.join(tempdir, ntpath.basename(awarduser.image.url)), os.path.join(tempdir, 'signature.jpg'))
        filename = os.path.join(tempdir, 'texput.tex')
        with open(filename, 'x', encoding='utf-8') as f:
            f.write(source)
        latex_interpreter = getattr(settings, 'LATEX_INTERPRETER', DEFAULT_INTERPRETER)
        latex_interpreter_options = getattr(settings, 'LATEX_INTERPRETER_OPTIONS', '')
        latex_command = f'cd "{tempdir}" && {latex_interpreter} -interaction=batchmode {latex_interpreter_options} {os.path.basename(filename)}'
        process = run(latex_command, shell=True, stdout=PIPE, stderr=PIPE)
        try:
            if process.returncode == 1:
                with open(os.path.join(tempdir, 'texput.log'), encoding='utf8') as f:
                    log = f.read()
                raise ValueError('A very specific bad thing happened')
            os.rename(os.path.join(tempdir, 'texput.pdf'), os.path.join(tempdir, 'awardCert.pdf'))
            certfile = tempdir + '\\awardCert.pdf'
            email = EmailMessage(
                subject='Congratulations',
                body='Congratulations on getting recognized',
                to=[award.recipient_email],
            )
            email.attach_file(certfile)
            email.send()
        except FileNotFoundError:
            if process.stderr:
                raise Exception(process.stderr.decode('utf-8'))
            raise


class AwardUpdateView(UpdateView):
    model = Award
    # fields = ['recipient_fname', 'recipient_lname', 'recipient_email', 'region', 'date_granted', 'category']
    form_class = AwardForm

    def form_valid(self, form):
        # self.helper.add_input(Submit('submit', 'create'))
        form.instance.submitter = self.request.user
        return super().form_valid(form)


class AwardDetailView(DetailView):
    model = Award


class AwardCreateView(CreateView):
    model = Award
    fields = ['recipient_fname', 'recipient_lname', 'recipient_email', 'region', 'date_granted', 'category']

    def form_valid(self, form):
        form.instance.submitter = self.request.user
        self.object=form.save()
        # print(self.object.id)
        sendpdf(self.object.id)
        return HttpResponseRedirect(self.get_success_url())



