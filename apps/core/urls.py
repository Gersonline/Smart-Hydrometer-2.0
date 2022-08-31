from django.urls import path, include
from apps.dashboard.views import load_dashboard

urlpatterns = [
    #path('', load_dashboard, name='load_dashboard'),
    path('', include('apps.dashboard.urls')),
]