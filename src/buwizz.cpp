#include <Arduino.h>
#include <BLEDevice.h>

#include <FunctionFSM.h>

#include "buwizz.h"

static BLEUUID BuwizzService( "4e050000-74fb-4481-88b3-9919b1676e93" );
static BLEUUID BuwizzC13s( (uint16_t) 0x92d1 );

union Packet
{
  struct {
    uint8_t magic;
    char    speed[4];
    uint8_t zero;
  } payload;
  uint8_t data[6];
} __attribute__((packed));

static union Packet packet;

void a_on_enter()
{
}

FunctionState state_a(&a_on_enter, nullptr, nullptr);

FunctionFsm fsm(&state_a);

class ScanCallbacks : public BLEAdvertisedDeviceCallbacks
{
public:
	void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Buwizz.on_adv_device(&advertisedDevice);
  }
};

ScanCallbacks scanCallbacks;

void BuwizzImpl::setup()
{
  Serial.printf("Init connection. Payload bytes: %d\n", sizeof(packet));

  BLEDevice::init("Ezdude");
  scan = BLEDevice::getScan();
  scan->setActiveScan(false);
  scan->setAdvertisedDeviceCallbacks(&scanCallbacks);
  
  client = BLEDevice::createClient();
  connection = nullptr;

scan:
  do {
    Serial.println("Start active scan...");
    scan->start(3);
  } while(!advdev);

  Serial.printf("Connecting...");

  while (!client->connect(advdev))
  {
    Serial.println("error, retrying...");

    delete advdev;
    advdev = nullptr;
    goto scan;
  }
  
  Serial.printf("OK\nGet service...");
  BLERemoteService* svc = client->getService(BuwizzService);
  if (svc) {
    Serial.printf("OK\nGetCharacteristic...");
    connection = svc->getCharacteristic(BuwizzC13s);
    if (connection) {
      Serial.printf("OK\nBuWizz connected!\n");
      return;
    }
  }

}


void BuwizzImpl::run()
{

}


void BuwizzImpl::set_mode(Mode mode)
{
  static uint8_t cmd[2] = { 0x11, 0x00 };
  cmd[1] = (int) mode;
  connection->writeValue(cmd, sizeof(cmd), true);
}


void BuwizzImpl::set_speed(char channel, char speed)
{
  Serial.printf("Set channel %d speed %d, bytes: %d\n", (int) channel, (int) speed, (int) sizeof(packet));
  packet.payload.speed[ (channel-1) & 0x3] = speed;
  connection->writeValue(packet.data, sizeof(packet), true);
}

void BuwizzImpl::on_adv_device(BLEAdvertisedDevice *dev)
{
  Serial.printf("Detected %s\n", dev->toString().c_str());
  if (!dev->getName().find("BuWizz"))
  {
    if (advdev) *advdev = *dev;
    else advdev = new BLEAdvertisedDevice(*dev);

    Serial.println("BuWizz found, stop scan");
    
    scan->stop();
  }

}

BuwizzImpl::BuwizzImpl()
{
		connection = nullptr;
		client = nullptr;
    scan = nullptr;
    advdev = nullptr;
    memset(packet.data, 0, sizeof(packet));
    packet.payload.magic = 0x10;
}

BuwizzImpl::~BuwizzImpl()
{
}
