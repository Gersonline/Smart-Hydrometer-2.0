# Generated by Django 4.1 on 2022-09-19 05:12

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('dashboard', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='hydrometer',
            name='macadress',
            field=models.CharField(blank=True, max_length=100, null=True),
        ),
        migrations.AlterField(
            model_name='hydrometer',
            name='password_ssid',
            field=models.CharField(blank=True, max_length=100, null=True),
        ),
        migrations.AlterField(
            model_name='hydrometer',
            name='ssid',
            field=models.CharField(blank=True, max_length=100, null=True),
        ),
        migrations.AlterField(
            model_name='hydrometer',
            name='version',
            field=models.CharField(blank=True, max_length=100, null=True),
        ),
    ]
