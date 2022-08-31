from django.urls import path
from django.views.generic import RedirectView
from .views import load_dashboard

urlpatterns = [
    path('', RedirectView.as_view(url='/dashboard/')),
    path('dashboard/', load_dashboard, name='dashboard'),
    path('dashboard/<int:id_hydrometer>', load_dashboard, name='dashboard'),

    #path('dashboard/', views.load_dashboard),
    #path('dashboard/logout/', views.logout_user),
]