from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.shortcuts import render, redirect

@login_required(login_url='/accounts/login/')
def load_profile(request):
    usuario = request.user

    return render(request, 'profile.html')
