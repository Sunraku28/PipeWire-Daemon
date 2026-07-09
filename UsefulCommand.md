# Useful Commands 

Here some commands of Pipewire and WirePlumber for the users of this tool.

## WirePlumber 
* **List all the clients, audio sinks, audio sources etc with there respective NODE ID and NAME.** : `wpctl status`
* **Display the current audio of the audio sink or audio stream.** : `wpctl get-volume <stream_id>`
* **Set volume of that Audio Stream(for eg. spotify) to the required number(eg. 0.5->50%,0.3->30%).** : `wpctl set-volume <stream_id> <volume_value>`
* **If the above commands dosen't help out use this to list all the commands of WirePlumber Controls.** : `wpctl help`

## PipeWire
Probably this will be the only commands you will require while using this tool.
* **To change the Output of a audio stream.(eg. Change spotify's output from speaker to headphone). Also this can done using "wpctl move" command.** : `pw-metadata <stream_id> target.object <target_sink_id>`