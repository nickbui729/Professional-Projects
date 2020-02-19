from django import forms
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm, UserChangeForm, PasswordResetForm
from django.utils.translation import gettext, gettext_lazy as _
from .models import Account
from .models import Profile


class AccountRegisterForm(UserCreationForm):
    """
    Responsible for the form to register. User fills the form, we process the form, and put it in DB.
    """
    class Meta(UserCreationForm):
        model = Account

        # Account attributes that will be filled in
        fields = ['email', 'first_name', 'last_name', 'password1', 'password2']

        # Overriding some settings and labels
        # this makes it so that we validate it's a proper email
        email = forms.CharField(required=True, widget=forms.EmailInput(attrs={'class': 'validate', }))

        # updates the labels that appears on the UI
        labels = {
            'first_name': _('First Name'),
            'last_name': _('Last Name'),
        }

    def save(self, commit=True):
        """
        This overrides the default 'save' function in UserCreationForm.
        The function creates an instance in our database, filling fields as appropriate.
        Of special note is we set the username to be same as email address that user gives us.
        """
        # this creates an Account model, but does not save to DB yet
        user = super().save(commit=False)

        # this sets the empty username to be same as inputted email
        user.username = user.email
        user.is_admin = user.is_staff
        user.set_password(self.cleaned_data["password1"])

        if commit:
            user.save()
        return user


class AccountChangeForm(UserChangeForm):
    class Meta:
        model = Account
        fields = ('email', 'password', 'first_name', 'last_name')


# These two update classes below will handle the data required to update profile info
# In our case, we have updates for the first name, last name, email as well as profile sig
# Two different classes are used because the Profile update handles images. 
# Note * we can maybe combine into one class?
class AccountUpdateForm(forms.ModelForm):
    email = forms.EmailField()

    first_name = forms.CharField(required=True, widget=forms.TextInput(attrs={'required': 'true'}),)
    last_name = forms.CharField(required=True, widget=forms.TextInput(attrs={'required': 'true'}),)

    class Meta:
        model = Account
        fields = ['first_name', 'last_name', 'email']


class ProfileUpdateForm(forms.ModelForm):
    class Meta:
        model = Profile
        fields = ['image']