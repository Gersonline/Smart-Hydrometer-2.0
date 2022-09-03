from django.contrib import admin
from .models import *

class MeterAdmin(admin.ModelAdmin):
    model = Meter

admin.site.register(Hydrometer)
admin.site.register(Meter)