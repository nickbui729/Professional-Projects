from django.contrib.auth.models import Permission
from django.contrib.contenttypes.models import ContentType
from django.db.models.signals import post_save, pre_save
from report_builder.models import Report

from .models import Account
from django.dispatch import receiver
from .models import Profile


# After a user registers, these signals are ran
# These signals will create a profile instance for us after a user is created 
# and also save it to the backend so we can access the data
@receiver(post_save, sender=Account)
def create_profile(sender, instance, created, **kwargs):
	if created:
		Profile.objects.create(user=instance)


@receiver(post_save, sender=Account)
def save_profile(sender, instance, **kwargs):
	instance.profile.save()


@receiver(post_save, sender=Account)
def set_admin_permissions(sender, instance, **kwargs):
	"""
	If registered account is admin, set permissions to able to create/edit/delete users
	"""
	if instance.is_staff and not instance.has_perm('account.view_account'):
		account_content_type = ContentType.objects.get_for_model(Account)
		add_account = Permission.objects.get(content_type=account_content_type, codename='add_account')
		change_account = Permission.objects.get(content_type=account_content_type, codename='change_account')
		delete_account = Permission.objects.get(content_type=account_content_type, codename='delete_account')
		view_account = Permission.objects.get(content_type=account_content_type, codename='view_account')

		report_content_type = ContentType.objects.get_for_model(Report)
		add_report = Permission.objects.get(content_type=report_content_type, codename='add_report')
		change_report = Permission.objects.get(content_type=report_content_type, codename='change_report')

		profile_content_type = ContentType.objects.get_for_model(Profile)
		delete_profile = Permission.objects.get(content_type=profile_content_type, codename='delete_profile')

		instance.user_permissions.set([
			add_account,
			change_account,
			delete_account,
			view_account,
			delete_profile,
			add_report,
			change_report
		])


@receiver(pre_save, sender=Account)
def auto_set_custom_attributes(sender, instance, **kwargs):
	"""
	This ensures that if we modify the email, it also updates the username to that email address (since we are treating
	username and emails as the same, even though they are different fields in the DB)
	"""
	instance.username = instance.email
	instance.is_admin = instance.is_staff
