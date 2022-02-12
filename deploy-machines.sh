Yellow='\033[0;33m'
White='\033[0;37m'

# Get book machine hostnames
echo -e "$Yellow[*] Get machines list$White"
cat $OAR_FILE_NODES | uniq > machinelist

touch machinefile

for machine in $(cat machinelist); do
    echo "$machine""   slots=17" >> machinefile;
done

# Deploy debian on each host
# Deploying can take about 5 minutes...
echo -e "$Yellow[*] Deploy debian on each host$White"
echo -e "$Yellow[*] Deploying can take about 5 minutes...$White"
kadeploy3 -f machinelist -k -e debian11-x64-base

echo -e "$Yellow[*] Install required packages for cloning project with git, ssh and tests in each host$White"
for machinename in $(cat machinelist); do
    ssh root@"$machinename" "apt-get update; \
        apt-get install -y sshpass git libopenmpi-dev bc python3 python3-pip python3-dev imagemagick; \
        pip install pillow; \
        git clone https://gitlab.ensimag.fr/elouatih/distributed.git; \
        cd distributed; \
        mpicc make.c -o make -fopenmp"
    scp machinefile root@"$machinename":distributed/machinefile
    # May have to generate and share rsa keys
done

echo -e "$Yellow[*] Change passwords & create SSH keys in each host $White"
for machinename in $(cat machinelist); do
    ssh root@"$machinename" "echo \"root:admin\" | chpasswd; \
                 ssh-keygen -t rsa -f .ssh/id_rsa -q -N \"\" "
done

# Exchange keys
echo -e "$Yellow[*] Copy SSH Key from each host to front-end  $White"
for machinename in $(cat machinelist); do
    scp root@"$machinename":.ssh/id_rsa.pub key_"$machinename";
done

echo -e "$Yellow[*] Copy SSH Key from front-end to other hosts  $White"
for machinename in $(cat machinelist); do
    for machinenamep in $(cat machinelist); do
      if [[ "$machinename" != "$machinenamep" ]] ; then
        scp key_"$machinename" root@"$machinenamep":.ssh;
      fi
    done
done

echo -e "$Yellow[*] Add other hosts' SSH keys to authorized_keys in each host  $White"
for machinename in $(cat machinelist); do
    for machinenamep in $(cat machinelist); do
          if [[ "$machinename" != "$machinenamep" ]] ; then
            ssh root@"$machinename" "cat .ssh/key_"$machinenamep" >> .ssh/authorized_keys"
          fi
        done
done

echo -e "$Yellow[*] You need to test connectivity between hosts  $White"
echo -e "$Yellow[*] before running the executable with mpirun.  $White"
echo -e "$Yellow[*] To test connectivity, create a file in each host  $White"
echo -e "$Yellow[*] and send it to other hosts after accepting   $White"
echo -e "$Yellow[*] every authentication request...   $White"
echo -e "$Yellow[*] -----------------------------------------   $White"
echo -e "$Yellow[*] Finish deploying on hosts  $White"