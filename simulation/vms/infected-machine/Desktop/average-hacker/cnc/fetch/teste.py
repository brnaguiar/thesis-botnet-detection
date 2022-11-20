import subprocess, os, urllib

result = os.popen("wget -O- http://127.0.0.1:4440/exploring")
output = result.read()[1:-1]
print(output)

""" reun command """

command = os.popen(output)
output_command = command.read()

print(output_command)


send_output_command = "wget --method=put -O- http://127.0.0.1:4440/task2?output=ola"
os.system(send_output_command)
