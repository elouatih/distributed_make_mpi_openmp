Yellow='\033[0;33m'
White='\033[0;37m'

# Get book machine hostnames
echo -e "$Yellow[*] Test connectivity between hosts$White"

for machinename in $(cat machinelist); do
    touch rien
    for machinenamep in $(cat machinelist); do
      if [[ "$machinename" != "$machinenamep" ]] ; then
        scp rien root@"$machinenamep":;
      fi
    done
done