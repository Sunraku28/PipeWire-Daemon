CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = audiorouter
SRCS = main.cpp NodeScanner.cpp CLI_Handler.cpp TargetSink_CHECK.cpp To_Lower.cpp
OBJS = $(SRCS:.cpp=.o)

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
SYSTEMD_USER_DIR = $(HOME)/.config/systemd/user

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(TARGET)
	@echo "[*] Installing binary to $(BINDIR)..."
	sudo install -m 755 $(TARGET) $(BINDIR)/$(TARGET)
	@echo "[*] Installing systemd service..."
	mkdir -p $(SYSTEMD_USER_DIR)
	cp audiorouter.service $(SYSTEMD_USER_DIR)/
	systemctl --user daemon-reload
	systemctl --user enable --now audiorouter.service
	@echo "[SUCCESS] audiorouter is now installed and running in the background!"

uninstall:
	@echo "[*] Stopping and disabling daemon..."
	systemctl --user disable --now audiorouter.service || true
	rm -f $(SYSTEMD_USER_DIR)/audiorouter.service
	systemctl --user daemon-reload
	@echo "[*] Removing binary..."
	sudo rm -f $(BINDIR)/$(TARGET)
	@echo "[SUCCESS] audiorouter uninstalled."

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all install uninstall clean