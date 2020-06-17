# ROSBridge2Unreal :robot: :bridge_at_night: :sparkles: :video_game:

This library implements the [ROSBridge Protocol](https://github.com/RobotWebTools/rosbridge_suite/blob/develop/ROSBRIDGE_PROTOCOL.md) inside [Unreal Engine](http://www.unrealengine.com). It supports the JSON and [BSON](http://bsonspec.org/) communication of ROSBridge.

## Credits to previous developers:

The implementation of this library is heavily inspired by [ROSIntegration](https://github.com/code-iai/ROSIntegration) and [rosbridge2cpp](https://github.com/sanic/rosbridge2cpp). It extends the functionality of these implementations by making its usage in Unreal's C++ and Blueprints easier. Additionally, the compatibility was improved by switching the dependencies to the header-only library [jsoncons](https://github.com/danielaparker/jsoncons).

## Installation:

### ROSBridge

To communicate with your ROS network, you need to start a ROSBridge node and enable communication with it. ROSBridge can be installed like this:

```shell
sudo apt-get install ros-kinetic-rosbridge-suite
```

and started like this (remove '#' to use BSON mode):

```shell
roslaunch rosbridge_server rosbridge_tcp.launch #bson_only_mode:=True
```

### Unreal Plugin

1. [Download the master branch of this repository](https://github.com/VRGroupRWTH/ROSBridge2Unreal/archive/master.zip) and extract it into `/Plugins/Rosbridge2Unreal` of your project.

2. Startup your project and let unreal build this plugin. After startup, activate the plugin in `Edit → Plugins → Project (At the bottom) → Other` if it isn't already.

3. Open the plugins settings in `Edit → Project Settings → Game → ROS Bridge` and enter the IP, Port and Transportation mode of your ROSBridge instance

4. Create an Actor as described in the usage of this plugin

## Usage Blueprints:

### Topics:

- Add a `ROSTopic Handle`-component to your actor

- **Subscribe** to a Topic like this: [Blueprint](https://blueprintue.com/blueprint/qy2wgm0p/). For this, do the following steps:
  
  - Call `Initialize` on your topic handle and enter the parameters
  
  - Call `Subscribe` on the same handle
  
  - Receive the `OnNewMessage`-Event by right clicking your topic in the hierarchy browser and `Add Event → OnNewMessage`
  
  - Cast the Message to your expected topic message type

- **Publish** to a Topic like this: [Blueprint](https://blueprintue.com/blueprint/sp8xpu8a/). For this, do the following steps:
  
  - Call `Initialize` on your topic and enter the parameters
  
  - Create a message of the correct message type. Most of the message feature a `Create` method
  
  - Call `Publish` with this message

### Services:

- Add a `ROSService Handle`-component to your actor

- **Call** a Service like this: [Blueprint](https://blueprintue.com/blueprint/rl5ysrx0/). For this, do the following steps:
  
  - Call `Initialize` on your service handle and enter the parameters
  
  - Create a service request. Most of the services feature a `Create Request` method
  
  - Call `Call` on the same handle with your request
  
  - Receive the `OnResponse`-Event by right clicking your service in the hierarchy browser and `Add Event → OnResponse`
  
  - Cast the Message to your expected service message type

- **Offer** a Service like this: [Blueprint](https://blueprintue.com/blueprint/g8-1gpg_/). For this, do the following steps:
  
  - Call `Initialize` on your service handle and enter the parameters
  
  - Call `Advertise` on your service handle
  
  - Receive the `OnRequest`-Event by right clicking your service in the hierarchy browser and `Add Event → OnRequest`
  
  - Cast the RequestResponse to your expected service type
  
  - Edit the RequestResponse to answer the service call appropriately
  
  - This object is automatically serialized and send back to ROS

## Usage C++:

### Topics:

- **Subscribe** to a Topic like this:

```cpp
  #include "Client.h"

  #include "Engine/Engine.h"
  #include "ROSMsgString.h"
  #include "ROSTopicHandle.h"

  AClient::AClient()
  {
      ReceiveTopic = CreateDefaultSubobject<UROSTopicHandle>("ReceiveTopic");
  }

  // Called when the game starts or when spawned
  void AClient::BeginPlay()
  {
      Super::BeginPlay();

      ReceiveTopic->Initialize("/chatter", UROSMsgString::StaticClass());

      ReceiveTopic->Subscribe(CallbackHelper::Subscribe<UROSMsgString>([](const UROSMsgString* Msg)
      {
          GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Cyan, "Received: " + Msg->Data);
      }));
  }
```

- **Publish** to a Topic like this:

```cpp
  #include "Client.h"

  #include "Engine/Engine.h"
  #include "ROSMsgString.h"
  #include "ROSTopicHandle.h"

  AClient::AClient()
  {
      SendTopic = CreateDefaultSubobject<UROSTopicHandle>("SendTopic");
  }

  // Called when the game starts or when spawned
  void AClient::BeginPlay()
  {
      Super::BeginPlay();

      SendTopic->Initialize("/chatter_unreal", UROSMsgString::StaticClass());
  }

  // Called every frame
  void AClient::Tick(float DeltaTime)
  {
      Super::Tick(DeltaTime);

      SendTopic->Publish(UROSMsgString::Create("Test" + FString::FromInt(Count)));

      Count++;    
  }
```

### Services:

- **Call** a Service like this:

```cpp
  #include "Client.h"

  #include "Engine/Engine.h"
  #include "ROSServiceHandle.h"
  #include "ROSSrvAddTwoInts.h"

  AClient::AClient()
  {
      ExternalService = CreateDefaultSubobject<UROSServiceHandle>("ExternalService");
  }

  // Called when the game starts or when spawned
  void AClient::BeginPlay()
  {
      Super::BeginPlay();

      ExternalService->Initialize("/add_two_ints", UROSSrvAddTwoInts::StaticClass());
      ExternalService->RegisterResponseCallback(CallbackHelper::Response<UROSSrvAddTwoInts>([](const UROSSrvAddTwoInts* Rsp)
      {
          GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Cyan, "Sum of two Ints: " + FString::FromInt(Rsp->Sum));
      }));
  }

  // Called every frame
  void AClient::Tick(float DeltaTime)
  {
      Super::Tick(DeltaTime);

      ExternalService->CallRecurrent(UROSSrvAddTwoInts::CreateRequest(Count, 2*Count));

      Count++;    
  }
```

- **Offer** a Service like this:

```cpp
  #include "Client.h"

  #include "Engine/Engine.h"
  #include "ROSServiceHandle.h"
  #include "ROSSrvAddTwoInts.h"

  AClient::AClient()
  {
      OfferedService = CreateDefaultSubobject<UROSServiceHandle>("OfferedService");
  }

  // Called when the game starts or when spawned
  void AClient::BeginPlay()
  {
      Super::BeginPlay();

      OfferedService->Initialize("/add_two_ints_unreal", UROSSrvAddTwoInts::StaticClass());
      OfferedService->Advertise(CallbackHelper::Advertise<UROSSrvAddTwoInts>([](UROSSrvAddTwoInts* Request)
      {
          Request->Sum = Request->A + Request->B;
      }));
  }
```

## Implement own Message-Types (on the example of `std_msgs/String`):

1. Add a C++ class that inherits from *UROSMessageBase* and use an appropriate naming scheme. This plugin uses `UROSMsg<YourMessageName>`.

2. Add "Rosbridge2Unreal" and "jsoncons" to the *PublicDependencyModuleNames* of your projects `*.Build.cs` file like this:
   
   ```csharp
   PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Rosbridge2Unreal", "jsoncons" });
   ```

3. Overwrite the `GetMessageType`, `ToData` and `FromData` functions of the parent class

4. The usage of the DataHelpers can be really handy here

5. The resulting code could look something like this:
```cpp
#include "CoreMinimal.h"
#include "ROSMessageBase.h"
#include "DataHelpers.h"
#include "ROSMsgString.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgString : public UROSMessageBase
{
    GENERATED_BODY()

public:
    /* Construction */
    UROSMsgString(){};
    UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/String";};
    UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgString* Create(FString Data);
    
    /* Data */
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Data;

    /* Transformation Functions */
    void ToData(ROSData& Message) const override;
    bool FromData(const ROSData& Message) override;
};

UROSMsgString* UROSMsgString::Create(FString Data)
{
    UROSMsgString* Message = NewObject<UROSMsgString>();
    Message->Data = Data;
    return Message;
}

void UROSMsgString::ToData(ROSData& Message) const
{
    DataHelpers::AppendString(Message, "data", Data);
}

bool UROSMsgString::FromData(const ROSData& Message)
{
    return DataHelpers::ExtractString(Message, "data", Data);
}
```

6. For more examples examine the `Rosbridge2Unreal/Source/Rosbridge2Unreal/<Private/Public>/Messages`

## Implement own Service-Types:

This implementation is very similar to the *Implement own Message-Types* chapter. For an example implementation of the `AddTwoInts`-Service look into `Rosbridge2Unreal/Source/Rosbridge2Unreal/<Private/Public>/Services/rospy_tutorials`

## Supported Message Types and Testing Status:

The following message types are pre-implemented. The tables show if the message types were implemented and tested already. If they were not tested, this does not mean, that they do not work, just that we did not have any use case to test them. If you tested one of them, report your testing result in [this Issue](https://github.com/VRGroupRWTH/ROSBridge2Unreal/issues/1), such that we can update this table.

| Topic Message Type                | Implemented | Tested |
| --------------------------------- | ----------- | ------ |
| std_msgs/Header                   | ✓           | ✘      |
| std_msgs/String                   | ✓           | ✓      |
| std_msgs/Float32                  | ✓           | ✘      |
| std_msgs/Float32MultiArray        | ✓           | ✘      |
| std_msgs/MultiArrayDimension      | ✓           | ✘      |
| std_msgs/MultiArrayLayout         | ✓           | ✘      |
| std_msgs/UInt8MultiArray          | ✓           | ✘      |
| geometry_msgs/Point               | ✓           | ✘      |
| geometry_msgs/Pose                | ✓           | ✘      |
| geometry_msgs/PoseStamped         | ✓           | ✘      |
| geometry_msgs/PoseWithCovariance  | ✓           | ✘      |
| geometry_msgs/Quaternion          | ✓           | ✘      |
| geometry_msgs/Transform           | ✓           | ✘      |
| geometry_msgs/TransformStamped    | ✓           | ✘      |
| geometry_msgs/Twist               | ✓           | ✘      |
| geometry_msgs/TwistWithCovariance | ✓           | ✘      |
| geometry_msgs/Vector3             | ✓           | ✘      |
| rosgraph_msgs/Clock               | ✓           | ✓      |

| Service Message Type       | Implemented | Tested |
| -------------------------- | ----------- | ------ |
| rospy_tutorials/AddTwoInts | ✓           | ✓      |

| Unreal Version    | Tested  |
| ----------------- | ------- |
| 4.22              | ✓       |

## Contribute:

We are happy for all contributors to this repository! :tada: 

You can help us in the following way:

- Feel free to fork it and issue a merge-request

- Report Bugs that you found as an Issue to this repository

- Create Issues with feature requests such that we or others can solve them

- Solve Issues for us

- Implement more of the standard message types and share them here with others

- Solve some points from the *Improvements in the future* section

## Improvements in the future:

- Implementation of more standard messages

- Support for CBOR communication
  
  - Is supported in jsoncons, not yet implemented

- Testing of all implementations

- ROS 2 Bridge support?

- Add back message queuing support. This is implemented, but not used at the moment.

### Additional Notes:

This project has received funding from the European Union’s Horizon 2020 research and innovation programme under grant agreement No. 871260
