import datetime, time, requests, json, psycopg2

def con_db():
    con = psycopg2.connect(host='localhost',
                           database='hydrometer',
                           user='postgres',
                           password='3301')
    return con

def ip_meters():
    sql = """SELECT id, ip, client_id FROM public.hydrometer where read_state = true"""
    con = con_db()
    cur = con.cursor()
    try:
        cur.execute(sql)
        ip_lines = cur.fetchall()
        for row in ip_lines:
            id = row[0]
            ip = row[1]
            client_id = row[2]
            #print(id, ip, client_id)
            insert_db(id, ip, client_id)
    except (Exception, psycopg2.DatabaseError) as error:
        print("Error: %s" % error)
        con.rollback()
        cur.close()
        return 1
    cur.close()



# Função para inserir dados no banco
def insert_db(id, ip, client_id):
    dt = datetime.datetime.now()
    try:
        requisition = requests.get(f'http://{ip}/api')
        dicRequisition = json.loads(requisition.content)
    except:
        return 1

    #print(dicRequisition.keys())
    ssid = dicRequisition['ssid']
    ip_hydrometer = dicRequisition['ip']
    totalMilliLitres = dicRequisition['totalMilliLitres']
    flowRate = dicRequisition['flowRate']
    macAddress = dicRequisition['macAddress']

    #print(totalMilliLitres, flowRate, dt)

    sql = """
           INSERT into public.meter (flowrate, datetime, hydrometer_id, client_id)
           values('%s','%s','%s','%s');
           """ %(totalMilliLitres, dt, id, client_id)
    sql = sql + """ 
            UPDATE public.hydrometer SET macadress = '%s'
             WHERE id = '%s' AND client_id = '%s'; 
             """ %(macAddress, id, client_id)

    #print(sql)
    con = con_db()
    cur = con.cursor()
    try:
        cur.execute(sql)
        con.commit()
    except (Exception, psycopg2.DatabaseError) as error:
        print("Error: %s" % error)
        con.rollback()
        cur.close()
        return 1
    cur.close()

n = 0
while True:
    ip_meters()

    '''
    last_meter = datetime.datetime.now()
    requisition = requests.get('http://192.168.1.102/api')
    dicRequisition = json.loads(requisition.content)

    print(dicRequisition.keys())
    ssid = dicRequisition['ssid']
    ip = dicRequisition['ip']
    totalMilliLitres = dicRequisition['totalMilliLitres']
    flowRate = dicRequisition['flowRate']

    print(totalMilliLitres, flowRate, last_meter)
    n = n + 1

    sql = """
        INSERT into public.flow_records (id, flowrate, first_meter, last_meter,  user_id)
        values('%s','%s','%s','%s','%s');
        """ %(n, totalMilliLitres, last_meter, last_meter, '1')
    '''
    #insert_db(sql)


    time.sleep(1)
