# Dynamic Audio Router (audiorouter)

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Linux](https://img.shields.io/badge/OS-Linux-orange.svg)]()
[![PipeWire](https://img.shields.io/badge/Audio-PipeWire-green.svg)]()

A lightweight, autonomous C++ background daemon and CLI utility for Linux. **audiorouter** solves the problem of volatile PipeWire/WirePlumber Node IDs by dynamically intercepting audio streams and automatically routing them to user-defined sinks with specific volume levels.

---

## Features

* **Dual-Mode Architecture:** Functions as both a background daemon and a quick CLI management tool.
* **Zero-Latency Autonomous Routing:** Intercepts PipeWire nodes in real-time; never lose your audio routing when an app restarts.
* **Persistent Configuration:** Saves rules in standard XDG paths (`~/.config/audiorouter/config.json`).
* **systemd Integration:** Installs as a user service for seamless boot initialization.
* **Resource Efficient:** Written in pure C++ using kernel pipes (`popen`) with minimal CPU footprint.

---

## Architecture & Flowchart

The application relies on a dual-path execution model. Terminal commands bypass the daemon to safely update the shared JSON configuration, which the daemon actively polls and applies to live system audio graphs.

```mermaid
graph TD
    %% Styling
    classDef cli fill:#2b3a42,stroke:#3b4d56,stroke-width:2px,color:#fff
    classDef daemon fill:#1a362a,stroke:#2a4d3c,stroke-width:2px,color:#fff
    classDef sys fill:#3f2a42,stroke:#5a3c5e,stroke-width:2px,color:#fff
    classDef file fill:#4a4a4a,stroke:#666,stroke-width:2px,color:#fff

    %% User Input
    User((User Terminal)) --> |audiorouter set...| CLI
    Boot((systemd Login)) --> |ExecStart| Daemon

    %% Modules
    subgraph "CLI Mode (Foreground)"
        CLI[CLI_Handler.cpp]:::cli --> |Parses args & formats JSON| Config[~/.config/audiorouter/config.json]:::file
    end

    subgraph "Daemon Mode (Background)"
        Daemon[main.cpp Loop]:::daemon --> |Polls rules| Config
        Daemon --> |Calls every 2s| Scanner[NodeScanner.cpp]:::daemon
        Scanner --> |popen 'wpctl status'| PipeWire[Linux PipeWire Graph]:::sys
        Scanner -.-> |Returns active Dictionary| Daemon
    end

    %% Execution
    Daemon --> |ID Match Detected| Route[pw-metadata <br/> Route Audio]:::sys
    Daemon --> |ID Match Detected| Vol[wpctl <br/> Set Volume]:::sys 
```

## Installation
### Requirements

* A Linux Distribution running PipeWire and WirePlumber.
* g++ compiler and make.

## Build and Install
Clone the repository and use the provided Makefile to compile and install the binary and system service globally

* `git clone [https://github.com/yourusername/audiorouter.git](https://github.com/yourusername/audiorouter.git)`
* `cd audiorouter`
* `make`
* `sudo make install`

## How to use
* **Run the daemon** : `audiorouter` 
* **Add or Update a rule** : `audiorouter set <app_name> <target_sink> <volume>`(eg audiorouter set spotify speaker 0.5)
* **List all configured application** : `audiorouter list`
* **Delete a configuration** : `audiorouter remove <app_name>` 
* **Update the volume of a configuration** : `audiorouter set-volume <app_name> <volume>`

> [!IMPORTANT]
> **How to Find Your Target Sink Name:**
> When setting a routing rule, PipeWire requires the internal system **`node.name`**, not the friendly display name.
> 
> To list the exact names of all your active audio sinks, run this command in your terminal:
> ```bash
> pactl list sinks short
> ```
> Copy the exact name from the second column (e.g., `alsa_output.pci-0000_00_1f.3.analog-stereo` or `Virtual_Speaker`) and pass it to `audiorouter`:
> ```bash
> audiorouter set spotify alsa_output.pci-0000_00_1f.3.analog-stereo 0.5
> ```
> Example
> ![Alt text](assets/Example_sink_list.png)

> [!NOTE]
> **How Volume Rules Work:**
> Volumes set by `audiorouter` apply directly to the application's individual audio stream, not your system's master volume slider. 
> 
> The final audible volume is a multiplication of both: if your Master Speaker is set to `50%` (`0.5`) and you route Spotify to `0.4` (`40%`), Spotify will play at `20%` of your hardware's total capacity (`0.5 × 0.4`). This ensures your keyboard's master volume controls always remain the global ceiling for all audio!


### Run it as a Background User Service(Recommended)
If you want that the tool turns on when you login the system just make it a user service via these commands :-

**->1. Ensure your personal systemd folder exists**
`mkdir -p ~/.config/systemd/user`

**->2. Copy the service file to your personal folder**
`cp audiorouter.service ~/.config/systemd/user/`

**->3. Tell your systemd to refresh and look for new files**
`systemctl --user daemon-reload`

**->4. Enable and start the daemon immediately**
`systemctl --user enable --now audiorouter.service`

## Uninstallation

To completely remove the binary, clear the systemd service, and stop the daemon safely.
* `sudo make uninstall`