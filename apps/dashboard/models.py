from django.db import models
from apps.accounts.models import Client
class Hydrometer(models.Model):
    description = models.CharField(max_length=100)
    macadress = models.CharField(max_length=100, blank=True, null=True)
    version = models.CharField(max_length=100, blank=True, null=True)
    ssid = models.CharField(max_length=100, blank=True, null=True)
    password_ssid = models.CharField(max_length=100, blank=True, null=True)
    ip = models.CharField(max_length=100, blank=False, null=False)
    read_state = models.BooleanField(blank=False, null=False, default=False)
    client = models.ForeignKey('accounts.Client', on_delete=models.PROTECT, null=False, blank=False)

    class Meta:
        db_table = 'hydrometer'

    def __str__(self):
        return self.description

class Meter(models.Model):
    flowrate = models.CharField(max_length=1000)
    datetime = models.DateTimeField(auto_now=True)
    hydrometer = models.ForeignKey('Hydrometer', on_delete=models.PROTECT)
    client = models.ForeignKey('accounts.Client', on_delete=models.PROTECT, null=False, blank=False)

    class Meta:
        db_table = 'meter'

    def __str__(self):
        return self.flowrate
