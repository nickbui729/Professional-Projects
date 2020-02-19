import django_tables2 as tables
from django_tables2.columns import checkboxcolumn
from .models import Award


class AwardTable(tables.Table):
    # selection = tables.CheckBoxColumn(accessor='pk', orderable=False)
    awardCat = tables.TemplateColumn('<a href="{% url \'award-detail\' record.id %}">{{record.category}}</a>',
                                     verbose_name='Category', order_by='category')
    edit = tables.TemplateColumn('<a class= "btn btn-outline-info" href="{% url \'award-update\' record.id %}">Edit</a>',
                                   orderable=False, verbose_name='')
    delete = tables.TemplateColumn('<a class= "btn btn-outline-info" href="{% url \'award-delete\' record.id %}">Delete</a>',
                                   orderable=False, verbose_name='')
    resendcert = tables.TemplateColumn('<a class= "btn btn-outline-info" href="{% url \'award-send\' record.id %}">Re-Send</a>',
                                   orderable=False, verbose_name='')

    class Meta:
        model = Award
        attrs = {'class': 'table'}
        fields = ('awardCat','recipient_fname', 'recipient_lname', 'recipient_email', 'date_granted',
                  'region')
        sequence = ('awardCat','recipient_fname', 'recipient_lname', 'recipient_email', 'date_granted',
                    'region', 'edit', 'delete', 'resendcert')
        template_name = 'django_tables2/bootstrap.html'

