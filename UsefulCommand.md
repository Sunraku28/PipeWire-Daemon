# Useful Commands 

Here some commands of Pipewire and WirePlumber for the users of this tool.

## WirePlumber 
* **wpctl status** : List all the clients, audio sinks, audio sources etc with there respective NODE ID and NAME.
* **wpctl get-volume** : Display the current audio of the audio sink or audio stream.
* **wpctl set-volume <stream_id> <volume> (for eg. 0.5)** : Set volume of that Audio Stream(for eg. spotify) to the required number.
* **wpctl help** : If the above commands dosen't help out use this to list all the commands of WirePlumber Controls.

## PipeWire
Probably this will be the only commands you will require while using this tool.
* **pw-metadata <stream_id> target.object <target_sink_id>** : To change the Output of a audio stream.(eg. Change spotify's output from speaker to headphone). Also this can done using "wpctl move" commands. 