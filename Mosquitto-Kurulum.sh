cd /root/HAP-NodeJS
if type mosquitto>/dev/null; then
      echo "Mosquitto zaten yüklü."
   else
      echo "Mosquitto bulunamadi. Kuruluyor."
        clear
         wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
         apt-key add mosquitto-repo.gpg.key
         rm -rf mosquitto-repo.gpg.key
        cd /etc/apt/sources.list.d/
         wget http://repo.mosquitto.org/debian/mosquitto-jessie.list
         apt update
         apt install mosquitto mosquitto-clients -y    
         
cd /root/HAP-NodeJS
npm install mqtt --save
cd accessories/
wget http://bit.ly/2vroCYu -O MQTTMotionSensor_accessory.js
clear
echo "Process Complete!"
