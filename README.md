# Turtle

Remote control car with IR driven obstacle avoidance and RF dynamic communication.

## API

The code is broken into 3 parts.

- `obstacleAvoidance.ino`: This code allows the bot to manuever a space using IR sensors to avoid obstacles. With this particular piece of code the bot simply moves straight at all times, but adjusts it's trajectory to avoid obstacles.
- `RemoteCarReciever.ino`: This code allows the bot to recieve RF communications from a transmitter and move accordingly. Note that this code implements a slightly modified version of the object avoidance code. Do not load both at the same time!
- `RemoteCarTransmitter.ino`: This code should be loaded on to a controller. It uses push buttons to send RF transmissions to the bot.

Images of the necessary circuit board configurations are included in the repo and explained in the code.

## Videos

You can see the obstacle avoidance in action [here](https://sites.google.com/a/mtholyoke.edu/cs-243-spring-15---reddy22k/home/assignments)

And the remote control in action [here](https://sites.google.com/a/mtholyoke.edu/cs-243-spring-15---reddy22k/home/project-2-remote-control-car)
