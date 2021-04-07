#ifndef __BUWIZZ_H__
#define __BUWIZZ_H__

#include <functional>
#include <BLEDevice.h>

#define Buwizz				(BuwizzImpl::get_instance())

#define	BW_MODE_SLOW		BuwizzImpl::Slow
#define	BW_MODE_NORMAL		BuwizzImpl::Normal
#define	BW_MODE_FAST		BuwizzImpl::Fast
#define	BW_MODE_LUDICROUS	BuwizzImpl::Ludicrous

class BLEAdvertisedDevice;
class BLERemoteCharacteristic;
class BLEScan;
class BLEClient;

class BuwizzImpl
{
public:

	static BuwizzImpl& get_instance()
	{
		static BuwizzImpl singleton;
		return singleton;
	}

	/**
	 * @brief Начальная настройка.
	 */
	void setup();

	/**
	 * @brief Периодические задачи.
	 * 
	 */
	void run();

	/**
	 * @brief Режим BuWizz
	 */
	enum Mode {
		Slow = 1,		// Медленный
		Normal = 2,		// Обычный
		Fast = 3,		// Быстрый
		Ludicrous = 4	// Смехотворный
	};

	/**
	 * @brief Установка режима.
	 * 
	 * @param mode режим
	 */
	void set_mode(Mode mode);

	/**
	 * @brief Установка скорости вращения мотора.
	 * 
	 * @param channel номер канала, 1..4
	 * @param speed скорость вращения -127..127
	 */
	void set_speed(char channel, char speed);

	/**
	 * @brief Обратный вызов с результатами сканирования.
	 *        Используется внутри класса.
	 * @internal
	 * 
	 * @param results список найденных устройств.
	 */
	void on_adv_device(BLEAdvertisedDevice *dev);

private:

	BuwizzImpl();
	~BuwizzImpl();
	BuwizzImpl(const BuwizzImpl&);                 // Prevent copy-construction
	BuwizzImpl& operator=(const BuwizzImpl&);      // Prevent assignment

	BLERemoteCharacteristic *connection;
	BLEScan					*scan;
	BLEClient				*client;
	BLEAdvertisedDevice		*advdev;
};

#endif//__BUWIZZ_H__
