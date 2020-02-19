from django.contrib import admin
from django.contrib.auth import get_user_model
from django.contrib.auth.admin import UserAdmin
from django.contrib.auth.models import Group
from django.utils.translation import gettext, gettext_lazy as _

from .forms import AccountChangeForm, AccountRegisterForm
from .models import Account, Profile


class AccountAdmin(UserAdmin):
    model = Account
    list_display = ['email', 'is_staff', 'first_name', 'last_name']

    # what admin sees when they attempt to add new user
    add_form = AccountRegisterForm
    add_fieldsets = (
        (None, {
            'classes': ('wide',),
            'fields': ('email', 'first_name', 'last_name', 'password1', 'password2', 'is_staff'),
        }),
    )

    form = AccountChangeForm
    fieldsets = (
        (None, {'fields': ('email', 'password')}),
        (_('Personal info'), {'fields': ('first_name', 'last_name')}),
    )


# admin.site.register(Profile) # we don't want to manage Profiles as an admin, only Accounts
admin.site.register(Account, AccountAdmin)

# Changes title of admin page
admin.site.site_header = "ERP Admin Portal"
admin.site.unregister(Group)
