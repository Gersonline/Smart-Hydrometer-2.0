from django.urls import path
from apps.dashboard.views import load_dashboard

urlpatterns = [
    path('', load_dashboard, name='load_dashboard'),
]