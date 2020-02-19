from django.conf.urls import url
from django.urls import path, include
from . import views


urlpatterns = [
    # main page after login
    path('profile/', views.profile, name='profile'),

    path('register/', views.register, name='register'),
    path('', include('django.contrib.auth.urls')),
    url(r'login_success/$', views.login_success, name='login_success')
]
