from django.urls import path
from . views import load_dashboard

urlpatterns = [
    path('', load_dashboard, name='load_dashboard'),

    #path('dashboard/', views.load_dashboard),
    #path('dashboard/logout/', views.logout_user),
]