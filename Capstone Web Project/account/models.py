from django.contrib.auth.models import AbstractUser
from django.core.validators import EmailValidator
from django.db import models
from django.utils.translation import gettext_lazy as _


class Account(AbstractUser):
    """
    ID, First name, Last name, Password in AbstractUser.
    Email overrides the default definitions of email in AbstractUser.
    is_admin and signature are our custom defined fields.
    """

    email = models.CharField(
        _('Email Address'),
        max_length=150,
        unique=True,
        help_text=_('Required. 150 characters or fewer. Letters, digits and @/./+/-/_ only.'),
        validators=[EmailValidator()],
        error_messages={
            'unique': _("An account with that email already exists."),
        },
    )

    is_admin = models.BooleanField(
        'is admin',
        default=False,
    )

    # holds name of signature image file, which is stored on disk
    signature = models.CharField(
        'signature',
        max_length=150,
        blank=True
    )

# This class will handle the instances for a user's profile page

class Profile(models.Model):
    user = models.OneToOneField(Account, on_delete=models.CASCADE)
    image = models.ImageField(verbose_name='Signature Image', default='default.jpg', upload_to='profile_pics')

    def __str__(self):
        return f'{self.user.username} Profile'