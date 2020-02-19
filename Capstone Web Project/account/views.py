from django.shortcuts import render, redirect
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from django.urls import reverse

from .forms import AccountRegisterForm, AccountUpdateForm, ProfileUpdateForm


def register(request):
    if request.method == 'POST':
        form = AccountRegisterForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username') 
            messages.success(request, f'Your account has been created. You are now able to log in')
            return redirect('login')
    else:
        form = AccountRegisterForm()
    return render(request, 'registration/register.html', {'form': form})


def login_success(request):
    """
    After login, redirects user differently based on whether they are regular or admin.
    """
    return redirect(reverse('admin:index')) if request.user.is_staff \
        else redirect('awards-home')


@login_required
def profile(request):

    # Check to see if POST request and does form validation for us as well
    if request.method == 'POST':
        u_form = AccountUpdateForm(request.POST, instance=request.user)
        p_form = ProfileUpdateForm(request.POST, request.FILES, instance=request.user.profile)
        
        if u_form.is_valid() and p_form.is_valid():
            u_form.save() # Save user info to backend
            p_form.save() # Save profile info to backend
            messages.success(request, f'Your account has been updated.')
            return redirect('profile') # After saving, redirect to profile page

    else:
        # Else we just render empty form to the screen
        u_form = AccountUpdateForm(instance=request.user)
        p_form = ProfileUpdateForm(instance=request.user.profile)

    context = {

        'u_form': u_form,
        'p_form': p_form
    }

    return render(request, 'profile.html', context)
