from django.db import models
from django.contrib.auth.models import User
from hydrometer_utils import choices

# Create your models here.
class Client(User):
    gener = models.CharField(max_length=10, blank=False)
    rg = models.CharField(max_length=30, blank=True)
    cpf = models.CharField(max_length=30, blank=True)

    class Meta:
        db_table = 'client'


class ClientAdress(models.Model):
    street = models.CharField(max_length=200)
    number = models.CharField(max_length=10)
    complement = models.CharField(max_length=100)
    cep = models.CharField(max_length=10)
    state = models.CharField(max_length=2, choices=choices.UF_CHOICES, default='SP')
    city = models.CharField(max_length=200, default='SP')
    country = models.CharField(max_length=30)
    client = models.ForeignKey('Client', on_delete=models.CASCADE)

    class Meta:
        db_table = 'client_adress'

    def __str__(self):
        return self.street

class Config(models.Model):
    bar = models.CharField(max_length=30, blank=False)
    mode = models.CharField(max_length=30, blank=True)
    client = models.ForeignKey('Client', on_delete=models.CASCADE)