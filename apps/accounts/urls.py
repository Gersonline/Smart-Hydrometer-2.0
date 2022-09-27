from django.urls import path
from django.views.generic import RedirectView
from .views import load_profile

urlpatterns = [
    path('', RedirectView.as_view(url='/dashboard/')),
    path('profile/', load_profile, name='profile'),

    #path('dashboard/', views.load_dashboard),
    #path('dashboard/logout/', views.logout_user),
]