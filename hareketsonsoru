cd /root/HAP-NodeJS
if type mosquitto>/dev/null; then
      echo "Mosquitto zaten kurulmus."
   else
      echo "Mosquitto bulunamadi, Mosquitto kuruluma basliyor."
      if cat /etc/os-release | grep -q "jessie"; then
        clear
        wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
        apt-key add mosquitto-repo.gpg.key
        rm -rf mosquitto-repo.gpg.key
        cd /etc/apt/sources.list.d/
        wget http://repo.mosquitto.org/debian/mosquitto-jessie.list
        apt update
        apt install mosquitto mosquitto-clients -y
      elif cat /etc/os-release | grep -q "stretch"; then
        clear
        echo "stretch set as raspbian release"
         wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
         apt-key add mosquitto-repo.gpg.key
         rm -rf mosquitto-repo.gpg.key
        cd /etc/apt/sources.list.d/
         wget http://repo.mosquitto.org/debian/mosquitto-stretch.list
         apt update
         apt install mosquitto mosquitto-clients -y
      fi
fi
cd /home/pi/HAP-NodeJS
sudo npm install mqtt --save
cd accessories/
wget http://bit.ly/2vroCYu -O MQTTMotionSensor_accessory.js
clear
echo "Islem Tamam!"
