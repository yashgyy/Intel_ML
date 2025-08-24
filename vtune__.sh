vtune -collect uarch-exploration workload.exe
vtune -report hw-events -format=csv -csv-delimiter=comma -report-output=uarch_workload.csv -group-by process-id
vtune -report hw-events -r r003ue -format=csv -csv-delimiter=comma -report-output=uarch_workload_main.csv -group-by process-id
vtune -report hw-events -r r003ue -format=csv -csv-delimiter=comma -report-output=uarch_workload_main.csv -group-by process-id

vtune -collect uarch-exploration -knob collect-memory-bandwidth=true workload.exe 
vtune -report hw-events -report-knob show-issues=false -r r001ue -format=csv -csv-delimiter=comma -report-output=uarch_workload_main_bandwidth.csv -group-by process-id