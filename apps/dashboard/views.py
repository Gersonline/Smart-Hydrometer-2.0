from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from .models import Meter
from django.shortcuts import render, redirect

@login_required(login_url='accounts/login/')
def load_dashboard(request):
    usuario = request.user
    #print(usuario)

    meter = Meter.objects.select_related('hydrometer').filter(client=usuario)
    #print(meter)

    dados = {'meters':meter}

    #print(dados)
    return render(request, 'dashboard.html', dados)

# Create your views here.
