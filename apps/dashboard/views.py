from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from .models import Meter, Hydrometer
from django.shortcuts import render, redirect

@login_required(login_url='/accounts/login/')
def load_dashboard(request, id_hydrometer=None):
    #id_hydrometer = 3
    usuario = request.user
    #print(usuario)
    if id_hydrometer:
        meter = Meter.objects.select_related('hydrometer').filter(client=usuario, hydrometer=id_hydrometer)
    else:
        hydrometer_id = Hydrometer.objects.select_related('client').filter(client=usuario).first()
        meter = Meter.objects.select_related('hydrometer').filter(client=usuario, hydrometer=hydrometer_id.id)

    hydrometer = Hydrometer.objects.select_related('client').filter(client=usuario)
    #print(meter)

    dados = {'meters':meter,
             'hydrometers':hydrometer}

    #print(dados)
    return render(request, 'dashboard.html', dados)
