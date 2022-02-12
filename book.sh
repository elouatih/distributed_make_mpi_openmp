# Connect to Grid5000 Grenoble frontend
# ssh grenoble.g5k

# Book 3 hosts for 2 hours
oarsub -I -t deploy -l nodes=4,walltime=2

# Once the booking has started, you can run ./deploy-machines.sh
