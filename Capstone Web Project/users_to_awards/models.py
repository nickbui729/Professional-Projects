from django.db import models

from awards.models import Award
from account.models import Account


class AccountToAward(models.Model):
    award_id = models.ForeignKey(
        Award,
        on_delete=models.CASCADE,
        null=True)

    account_id = models.ForeignKey(
        Account,
        on_delete=models.CASCADE,
        null=True)
