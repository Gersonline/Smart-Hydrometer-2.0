from django.urls import path
from django.views.generic import RedirectView
#from .views import home

urlpatterns = [
    #path('', home, name='home'),
    path('', RedirectView.as_view(url='dashboard/')),
]