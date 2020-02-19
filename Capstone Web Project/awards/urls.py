# copied from the project-level urls.py file
from django.urls import path
from django.contrib.auth.decorators import login_required
from .views import (AwardCreateView,
                    AwardDetailView,
                    AwardUpdateView)
# from current directory import the views
from . import views


# create path for awards homepage. path() takes three arguments:
# First is the url route, which in this case will be empty path because the
# project-level url will lead to 'awards/' and the awards-level url should
# just be an empty path.  The second argument is the home view function
# from the awards view file, and the third argument is just a name we give to the path
# in the event we want to do a reverse lookup on the entire path
urlpatterns = [
    path('', views.home, name='awards-home'),
    # path('pdf/<int:pk>/', views.generatepdf, name='award-generatepdf'),
    path('send/<int:pk>/', views.send, name='award-send'),
    path('delete/<int:pk>/', views.delete, name='award-delete'),
    path('<int:pk>/', login_required(AwardDetailView.as_view()), name='award-detail'),
    path('update/<int:pk>/', login_required(AwardUpdateView.as_view()), name='award-update'),
    path('newAward/', login_required(AwardCreateView.as_view()), name='award-create')

]
