# IRC 42 Project

Welcome to the **IRC 42 Project**! This project is an implementation of an Internet Relay Chat (IRC) server, designed as part of the curriculum at 42 School. The goal of this project is to create a fully functional IRC server that adheres to the RFC 1459 standard, allowing multiple clients to connect, communicate, and interact in real-time.

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Commands](#commands)
6. [License](#license)
7. [Acknowledgments](#acknowledgments)

## Introduction

IRC (Internet Relay Chat) is a protocol for real-time text messaging between users on a network. This project involves building an IRC server from scratch, capable of handling multiple clients, channels, and basic IRC commands. The server is designed to be lightweight, efficient, and compliant with the IRC protocol standards.

## Features

- **Multi-client support**: Handle multiple clients connected simultaneously.
- **Channel management**: Create, join, and manage channels.
- **Basic IRC commands**: Support for commands like `/NICK`, `/JOIN`, `/PART`, `/PRIVMSG`, `/QUIT`, `/KICK`, `/INVITE`, `/MODE`, `/PING`, `/PONG`, `/TOPIC`, and more.
- **User authentication**: Simple user authentication using password, unique nickname, and username.
- **Real-time messaging**: Send and receive messages in real-time.
- **Cross-platform**: Compatible with any IRC client that adheres to the RFC 1459 standard.

## Installation

To get started with the IRC 42 Project, follow these steps:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ismail/ft_irc.git
   cd ft_irc
   ```

2. **Build the project**:
   ```bash
   make
   ```

3. **Run the server**:
   ```bash
   ./ircserver <port> <password>
   ```
   Replace `<port>` with the desired port number for the server to listen on.
   Replace `<password>` with a password.

## Usage

Once the server is running, you can connect to it using any IRC client. For example, using the `irssi`, `nc`, `LimeChat Application`clients:

```bash
nc -c localhost <port>
```

Replace `<port>` with the port number you specified when starting the server.

### Example Commands

- **First Authenticate**:
  ```
  pass <password>
  nick <unique_nickname>
  user <4 random argument>
  ```

- **Set your nickname**:
  ```
  NICK your_nickname
  ```

- **Join a channel**:
  ```
  JOIN #channel_name
  ```

- **Send a message to a channel**:
  ```
  PRIVMSG #channel_name :Hello, world!
  ```

- **Leave a channel**:
  ```
  PART #channel_name
  ```

- **Quit the server**:
  ```
  QUIT
  ```

## Commands

The IRC 42 server supports the following commands:

`<>` means required, `[]` optional

- `NICK <nickname>`: Set or change your nickname.
- `JOIN <channel> [password]`: Join a channel.
- `PART <channel>`: Leave a channel.
- `PRIVMSG <target> <message>`: Send a message to a channel or user.
- `QUIT [message]`: Disconnect from the server.
- `MODE <channel> <mode> [parameter]`: Change the mode of a channel.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgments

- **42 School**: For providing the opportunity and resources to work on this project.
- **RFC 1459**: The official IRC protocol specification.

---

Thank you for checking out the IRC 42 Project! We hope you find it useful and enjoyable. If you have any questions or feedback, please feel free to open an issue or reach out to the maintainers.

Happy chatting! 🚀
