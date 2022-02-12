Yellow='\033[0;33m'
White='\033[0;37m'
zero="0."
touch ./distributed/resultfile
echo -e "$Yellow[*] Run the executable in master host  $White"
# Run the main program
#ssh root@$(cat machinelist | head -n 1) "mpirun --allow-run-as-root -machinefile ./distributed/machinefile -np 13 ./distributed/make ./distributed/tests/Makefile6/premier"
#mpirun --allow-run-as-root -machinefile ./distributed/machinefile -np 13 ./distributed/make "$1"


for i in {2..68}
do
  result="$(time (ssh root@$(cat machinelist | head -n 1) "mpirun --allow-run-as-root -machinefile ./distributed/machinefile -np $i ./distributed/make ./distributed/tests/premier_simple") 2>&1 1>/dev/null)"
  #ssh root@$(cat machinelist | head -n 1) "mpirun --allow-run-as-root -machinefile ./distributed/machinefile -np $i ./distributed/make ./distributed/tests/Makefile_gif"
  result1=`echo $result| cut -c10-12`
  echo "$i,$result" >> ./distributed/resultfile
  echo -e "$Yellow[*] Tested with $i workers$White"
done

python3 ./distributed/stats.py 1
echo -e "$Yellow[*] Image generated ... $White"