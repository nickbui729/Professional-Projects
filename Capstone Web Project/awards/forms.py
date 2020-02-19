from django import forms
from django.forms import ModelForm
from crispy_forms.helper import FormHelper
from crispy_forms.layout import Layout
from crispy_forms.layout import Submit
from crispy_forms.layout import Field
from crispy_forms.layout import ButtonHolder
from .models import Award

class AwardForm(forms.ModelForm):
    #helper = FormHelper

    def __init__(self, *args, **kwargs):
        self.helper = FormHelper()
        self.helper.layout = Layout(
            Field( 'recipient_fname', 'recipient_lname', 'recipient_email', 'region', 'date_granted', 'category'),
            ButtonHolder(
                Submit('save', 'Update', css_class='btn btn-outline-info')
            )
        )
        super(AwardForm, self).__init__(*args, **kwargs)
        #self.helper.add_input(Submit('submit', 'Submit'))

    class Meta:
        model = Award
        fields = ['recipient_fname', 'recipient_lname', 'recipient_email', 'region', 'date_granted', 'category']
