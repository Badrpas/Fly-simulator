# Fly simulator

[![Build Status](https://travis-ci.com/Badrpas/Fly-simulator.svg?branch=master)](https://travis-ci.com/Badrpas/Fly-simulator)
[![BCH compliance](https://bettercodehub.com/edge/badge/Badrpas/Fly-simulator?branch=master)](https://bettercodehub.com/)

Magnificient application that allows you to control a perfectly drawed-in-paint fly that flies above a carpet.

# This thing even got a multiplayer!
So you can grab your friends and fly your flies together.


# To fulfill your urge to control the fly or whatever
- Head to the `/Debug/onlyFly/` directory
- Start server by executing `server.exe` command.
- Connect to server with `fly.exe $YOUR_SERVER_IP 3879` command.
- Use `arrow keys` to turn left right and increase/decrese altitude.
- Enjoy your journey!

You can also press `m` to see relative view position.

But you better not press other buttons because some of them are crashing client :neutral_face:

#### Warning for explorers
Do not expect to see a well written code here.

## Requirements

### Linux
#### Ubuntu
    apt-get install build-essential libsdl-image1.2-dev libsdl-dev libsdl-net1.2 libsdl-net1.2-dev
    
### Mac OS
    brew install cmake gcc sdl sdl_net sdl_image

## Build

```
mkdir build && cd build
cmake ..
make
```

P.S. don't forget to copy `images` folder into `Show me image`
