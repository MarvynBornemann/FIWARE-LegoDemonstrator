import paho.mqtt.client as paho
broker="localhost"
port=1883
username="LegoDemonstrator"
password="Lego12Demo34nstr56ator"
def on_publish(client,userdata,result):             #create function for callback
    print("data published \n")
    pass
client1= paho.Client("control1")                           #create client object
client1.on_publish = on_publish                          #assign function to callback
client1.username_pw_set(username, password)
client1.connect(broker,port)                                 #establish connection
ret= client1.publish("/idFZy8D9KzFko7db/Video001/cmd","{'play':''}")                   #publish
print(ret)