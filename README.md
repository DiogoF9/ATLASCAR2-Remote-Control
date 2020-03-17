# ATLASCAR2RemoteControl
ATLASCAR2 remote control and car status monitoring. 

(this README file is being updated as the work is done)

# Table of Contents

- [ATLASCAR2RemoteControl](#ATLASCAR2RemoteControl)
- [Table of Contents](#table-of-contents)
- [Installation](#installation)
- [Usage Instructions](#usage-instructions)
- [Credits](#credits)

# Installation
It will be needed the linux support for the CAN communication protocol.

```bash
sudo apt-get install can-utils
```

# Usage Instructions
In order to view some of the ATLASCAR2 paramenters status in real time run the follow command:

```bash
rosrun atlascar2 canReceiveAndUpdateStatus
```
This node will create a topic that contains information related to the car status, such as velocity and orientation. This topic can be seen running:

```bash
rostopic echo /NominalData
```

The ROS node canReveiveAndUpdateStatus also create a topic with all the CAN messages on the bus.

```bash
rostopic echo /RawFrames
```



# Credits

Author Name  | Email
------------- | -------------
Diogo Figueiredo | diogof@ua.pt
