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
    Daemon --> |ID Match Detected!| Route[pw-metadata <br/> Route Audio]:::sys
    Daemon --> |ID Match Detected!| Vol[wpctl <br/> Set Volume]:::sys