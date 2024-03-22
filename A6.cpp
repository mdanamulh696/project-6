/***********************************************************************
Matr.-Nr:                     3085596
Nachname/Surname:             Haque
Vorname/Given name:           Md Anamul
Uni-Email:                    md.haque.696@stud.uni-due.de
Studiengang/Course of studis: ISE(Software Engineering)
***********************************************************************/
#include<string>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<iostream>

const int annualaverageconsumptionperperson = 200;
const int annualaverageconsumptionpersquaremeter = 9;
const int annual_average_consumption_per_person_with_electric_water_heating = 550;
const int columnwidth = 35;
using namespace std;

class Device
{
 private:
    string description;
    Device* next; //to create list of power consumers and producers
 public:
    Device(Device* device = nullptr) { next = device;}
    
    // Public virtual destructor
    virtual ~Device() { cout << "Device " << this->get_description() << " at address " << this << " deleted" << '\n'; }
   
    //getters
    string get_description() { return this->description; }
    Device* get_next() { return this->next; }
   
    //setters
    void set_description(string description) { this->description = description; }
    void set_next(Device* next) { this->next = next; }
    virtual void input()
    {
        string description;
        cin.ignore();
        cout << "what is the description of the power consumer ?";
        getline(cin, this->description);
    }
    
    // Public pure virtual member function  
    virtual void print(int, float) = 0;
    virtual float annual_kWh() = 0;
    virtual Device* get_a_copy() = 0;
    virtual string get_string_for_file(char) = 0;
};

enum  Use
{
    once,
    daily,
    mo_fr,
    sa_su,
    weekly
};


class Consumer : public Device
{
 private:
    float watt;
    Use use;
    static int total_number; //store number of objects

 public:
    // overloded constructor
    Consumer(Device* device = nullptr):Device(device) { this->total_number++; }

    // virtual destructor
    virtual ~Consumer() override
    {
        cout << "Consumer " << this->get_description() << " at address " << this << " deleted\n";
        this->total_number--;
    }

    // getter method
    float get_watt() { return this->watt; }
    Use get_use() { return this->use; }
    static int get_total_number() { return total_number; }

    // setter method
    void set_watt(float watt) { this->watt = watt; }
    void set_use(Use use) { this->use = use; }

    // Public member functions
    void input() override
    {
        Device::input();
        cout << "how many watt it will have? ";
        cin >> watt;

        cout << "how often it will be used?";
        cout << "(d) daily";
        cout << "(m) mo_fr";
        cout << "(o) once";
        cout << "(s) sa_su";
        cout << "(w) weekly";

        cout << "how often is it used?\n(d) daily\n(m) mo_fr\n(o) once\n(s) sa_su\n(w) weekly? ";
        char choice;
        cin >> choice;

        switch (choice) {
        case 'd': use = daily;  break;
        case 'm': use = mo_fr;  break;
        case 'o': use = once;   break;
        case 's': use = sa_su;  break;
        case 'w': use = weekly; break;
        default:
            cout << "invalid choice. using 'once' as default." << endl;
            use = once;
            break;
        }
        this->use = use;
    }
    string getUseString(Use use)
    {
        string useStr;
        switch (use)
        {
            case Use::once:   useStr += "once"; break;
            case Use::daily:  useStr += "daily"; break;
            case Use::mo_fr:  useStr += "monday to friday"; break;
            case Use::sa_su:  useStr += "saturday and sunday"; break;
            case Use::weekly: useStr += "weekly";
        }
        return useStr;
    }

    
};

int Consumer::total_number = 0;

class Immobile : public Consumer
{
 private:
    float watt_standby;
    float hours;

 public:
    Immobile(Device* device = nullptr):Consumer(device) { watt_standby = hours = 0.0; }

   virtual ~Immobile() override { }

    float get_watt_standby() { return watt_standby; }
    float get_hours() { return hours; }

    void set_watt_standby(float watt_standby) { this->watt_standby = watt_standby; }
    void set_hours(float hours) { this->hours = hours; }

    float annual_hours_of_use()
    {
        switch (this->get_use())
        {
            case once: return this->hours;
            case daily: return this->hours * 365;
            case mo_fr: return this->hours * 260;
            case sa_su: return this->hours * 104;
            case weekly: return this->hours * 52;
            default: return 0;
        }
    }
    float annual_hours_of_standby() { return 8760 - this->annual_hours_of_use(); }

    float annual_kWh() override
    {
        return (((this->annual_hours_of_use() * this->get_watt()) + (this->annual_hours_of_standby() * this->watt_standby)) / 1000);
    }

    void  input()
    {
        Consumer::input();
        cout << "how many watt standby it will have? ";
        cin >> watt_standby;

        cout << "how many hours will it be operating? ";
        cin >> hours;

        this->set_watt_standby(watt_standby);
        this->set_hours(hours);
    }
    void  print(int index, float pricePerKWh)
    {
        cout << right << setw(columnwidth) << index << ": " << left << this->get_description() << endl;
        cout << right << setw(columnwidth) << "  (this" << ": " << left << this << ")" << endl;
        cout << right << setw(columnwidth) << "power consumption" << ": " << left << this->get_watt() << " w" << endl;
        cout << right << setw(columnwidth) << "power consumption standby" << ": " << left << this->watt_standby << " w" << endl;
        cout << right << setw(columnwidth) << "annual hours of use" << ": " << left << this->annual_hours_of_use() << " h" << endl;
        cout << right << setw(columnwidth) << "annual hours of standby" << ": " << left << annual_hours_of_standby() << " h" << endl;
        cout << right << setw(columnwidth) << "annual consumption" << ": " << left << this->annual_kWh() << " kWh" << endl;
        cout << right << setw(columnwidth) << "annual cost" << ": " << left << this->annual_kWh() * pricePerKWh << " EUR" << endl;
    }

    virtual Device* get_a_copy()
    {
        Immobile* copy = new Immobile();
        *copy = *this;
        return copy;
    }

    virtual string get_string_for_file(char separator) override
    {
        string s = "Immobile";
        s += separator + get_description() + separator + to_string(get_hours()) +
            separator;
        Use use = get_use();
        string useStr = getUseString(use);
        
        s += useStr + separator + to_string(get_watt()) + separator + to_string(get_watt_standby());
        return s;
    }
};


class Mobile : public Consumer
{
 private:
    float km;

 public:
    Mobile(Device* device = nullptr) :Consumer(device) { km = 0.0; }
    virtual ~Mobile() override { cout << "Mobile " << this->get_description() << " at address " << this << " deleted" << endl; }

    float get_km() { return this->km; }
    void set_km(float km) { this->km = km; }

    void input()
    {
        Consumer::input();
        cout << "how many km will be driven? ";
        cin >> this->km;
    }
    float annual_kWh() override { return (this->get_watt() * this->km * annual_hours_of_use()) / 100000.0; }

    float annual_hours_of_use()
    {
        switch (this->get_use())
        {
            case once: return 1;
            case daily: return 365;
            case mo_fr: return 260;
            case sa_su: return 104;
            case weekly: return 52;
            default: return 0;
        }
    }

    virtual void print(int number, float pricePerKWh) override
    {
        cout << right << setw(columnwidth) << number << ": " << left << this->get_description() << endl;
        cout << right << setw(columnwidth) << "  (this" << ": " << left << this << ")" << endl;
        cout << right << setw(columnwidth) << "power consumption 100 Km" << ": " << left << this->get_watt() / 1000 << " kW" << endl;
        string useStr = this->getUseString(this->get_use());
        cout << right << setw(columnwidth) << "Km driven" << ": " << left << this->get_km() << " Km " << useStr << endl;
        cout << right << setw(columnwidth) << "annual consumption" << ": " << left << this->annual_kWh() << " kWh" << endl;
        cout << right << setw(columnwidth) << "annual cost" << ": " << left << this->annual_kWh() * pricePerKWh << " EUR" << endl;
    }

    virtual Device* get_a_copy()
    {
        Mobile* copy = new Mobile();
        *copy = *this;
        return copy;
    }


    virtual string get_string_for_file(char separator) override
    {
        string s = "Mobile";
        s += separator + get_description() + separator + to_string(get_km()) +
            separator;

        Use use = get_use();
        string useStr = getUseString(use);
        s += useStr + separator + to_string(this->get_watt());
        return s;
    }
};


class Producer: public Device
{
 public:
    Producer(Device* device = nullptr) : Device(device) { }
    virtual ~Producer() { cout << "Producer " << this->get_description() << "at address " << this << " deleted " << endl; }
    void input() { Device::input(); } 
};

class Solar :public Producer
{
 private:
    float watt_peak;
    int year;
 public:
    Solar(Device* device = nullptr) :Producer(device) { watt_peak = year = 0; }
    virtual ~Solar() { cout << "Solar " << this->get_description() << "at address " << this << " deleted " << endl; }
    
    //get_watt_peak, get_year, set_watt_peak and set_year,
    float get_watt_peak() { return this->watt_peak; }
    int get_year() { return this->year; }
    void set_watt_peak(float watt_peak) { this->watt_peak = watt_peak; }
    void set_year(int year) { this->year = year; }

    void input()
    {
        Producer::input();
        cout << "how many watt peak have been installed? ";
        cin >> watt_peak;
        cout << "in which year the solar modules are installed? ";
        cin >> year;
    }

    virtual float annual_kWh() override
    {
        int diff = 2024 - this->year;
        float loss = 1.0 - (0.005 * diff);
        return (this->watt_peak * loss) * -1;
    }

    virtual void print(int deviceNumber, float pricePerKWh) override
    {

        cout << right << setw(columnwidth) << deviceNumber << ": " << left << this->get_description() << endl;
        cout << right << setw(columnwidth) << "  (this" << ": " << left << this << ")" << endl;
        cout << right << setw(columnwidth) << "solar cells installed power" << ": " << left << this->get_watt_peak() << " watt_peak" << endl;
        cout << right << setw(columnwidth) << "age of solar cells" << ": " << left <<  2024 - this->year << " years" << endl;
        int annual_generation = this->annual_kWh();
        cout << right << setw(columnwidth) << "annual production" << ": " << left << annual_generation << " kWh" << endl;
        cout << right << setw(columnwidth) << "annual savings" << ": " << left << annual_generation * pricePerKWh << " EUR" << endl;
    }
    virtual Device* get_a_copy()
    {
        Solar* copy = new Solar();
        *copy = *this;
        return copy;
    }

    virtual string get_string_for_file(char separator)
    {
        string s = "Solar";
        s += separator + this->get_description() + separator + to_string(this->get_year()) + separator + to_string(this->get_watt_peak());
        return s;
    }

};
class Household
{
 private:
    float price_kWh;
    string power_supplier;
    int persons;
    int square_meters;
    bool electric_water_heating;
    Device* devices; 
 public:
    Household() { devices = nullptr; }

    // Getter methods
    float get_price_kWh() { return price_kWh; }
    string get_power_supplier() { return power_supplier; }
    int get_persons() { return persons; }
    int get_square_meters() { return square_meters; }
    bool get_electric_water_heating() { return electric_water_heating; }
    Device* get_devices() { return devices; } 

    // Setter methods
    void set_price_kWh(float price) { price_kWh = price; }
    void set_power_supplier(string& supplier) { power_supplier = supplier; }
    void set_persons(int numPersons) { persons = numPersons; }
    void set_square_meters(int area) { square_meters = area; }
    void set_electric_water_heating(bool heating) { electric_water_heating = heating; }
    void set_devices(Device* newDevices) { devices = newDevices; } 

    // Public member functions
    void add_device_to_household(Device* newdevice, bool readingFromFile);
    Device* move_up(int k); // 9. move_up power consumer from consumer list
    Household* copy_devices(Household* source_house);
    void input_household();
    void printhousehold(int number_of_household);

    ~Household()
    {
        // Delete all power consumers
        Device* currentDevice = devices;
        while (currentDevice != nullptr)
        {
            Device* nextDevice = currentDevice->get_next();
            delete currentDevice;
            currentDevice = nextDevice;
        }
        cout << "Household at address " << this << " deleted\n";

    }

};

void Household::add_device_to_household(Device* newdevice, bool readingFromFile)
{
    if (readingFromFile)
    {
        if (!this->devices) { this->devices = newdevice; }
        else
        {
            Device* temp = this->devices;
            while (temp->get_next()) temp = temp->get_next();
            temp->set_next(newdevice);
        }
    }
    else
    {
        if (!this->devices) { this->devices = newdevice; }
        else
        {
            newdevice->set_next(this->devices);
            this->devices = newdevice;
        }
    }
}


// 9. move_up power consumer from consumer list
Device* Household::move_up(int k)
{
    Device* current_head = this->devices;
    if (k <= 1) { return current_head; }
    else
    {
        Device* current = current_head;
        Device* next_device = current->get_next();

        if (k == 2 && next_device != nullptr)
        {
            current->set_next(next_device->get_next());
            next_device->set_next(current_head);
            current_head = next_device;
            return current_head;
        }
        else
        {

            while (current != nullptr)
            {
                int i = 1;
                while (i < k - 2)
                {
                    if (current->get_next() == nullptr)
                    {
                        return current_head;
                    }
                    current = current->get_next();
                    i++;
                }

                if (current->get_next() == nullptr || current->get_next()->get_next() == nullptr)
                {
                    return current_head;
                }

                Device* kth_node = current->get_next()->get_next();
                current->get_next()->set_next(kth_node->get_next());
                kth_node->set_next(current->get_next());
                current->set_next(kth_node);

                return current_head;
            }

        }
    }

}

Household* Household::copy_devices(Household* source_house)
{
    // copy all devices from (source_house to destination_house)
    Device* temp_house_device = source_house->devices;
    Device* destination_house_device = this->devices;
    Device* temp_head = nullptr;
    Device* temp_tail = nullptr;

    if (temp_house_device == nullptr)
    {
        return this;
    }
    while (temp_house_device != nullptr)
    {
        Device* new_device = temp_house_device->get_a_copy();
        
        new_device->set_next(nullptr);

        if (temp_head == nullptr)
        {
            temp_head = new_device;
            temp_tail = new_device;
        }
        else
        {
            temp_tail->set_next(new_device);
            temp_tail = new_device;
        }

        temp_house_device = temp_house_device->get_next();
    }

    temp_tail->set_next(this->devices);
    this->devices = temp_head;
    return this;
}

void Household::input_household()
{
    char hotwater;

    cout << "how many square meters does the household have: ";
    cin >> this->square_meters;
    cout << "how many persons live in this household?";
    cin >> this->persons;
    cout << "is hot water heated using electricity? (y(es) or n(o)) ";
    cin >> hotwater;
    cout << "what is the price for one kwh in eur? ";
    cin >> this->price_kWh;
    cout << "who is the power supplier? ";
    cin.ignore();
    getline(cin, this->power_supplier);

    if (hotwater == 'y')
        this->electric_water_heating = true;
    else
        this->electric_water_heating = false;
}

// funtion print household
void Household::printhousehold(int number_of_household)
{
    string heater = (this->get_electric_water_heating()) ? "yes" : "no";

    cout << "h o u s e h o l d  n o  " << " " << number_of_household << "  p o w e r  c o n s u m p t i o n" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << right << setw(columnwidth) << "  (this" << ": " << left << this << endl;
    cout << right << setw(columnwidth) << "price for one kwh" << ": " << left << setprecision(2) << fixed << this->price_kWh*100.0 << " ct/kwh" << endl;
    cout << right << setw(columnwidth) << "power supplier" << ": " << left << this->power_supplier << endl;
    cout << right << setw(columnwidth) << "square meters" << ": " << left << this->square_meters << " qm" << endl;
    cout << right << setw(columnwidth) << "persons" << ": " << left << this->persons << endl;
    cout << right << setw(columnwidth) << "water heated using electricity" << ": " << left << heater << endl;
    cout << right << setw(columnwidth) << "list of devices" << ": " << endl;
    cout << "---------------------------------------------------------------------" << endl;

    float powerconsumptionsquare_meters = (this->square_meters * annualaverageconsumptionpersquaremeter);
    float powerconsumptionpersons = (this->persons * (this->electric_water_heating ? annual_average_consumption_per_person_with_electric_water_heating : annualaverageconsumptionperperson)); // using ternary operator
    float totalconsumption = (powerconsumptionsquare_meters + powerconsumptionpersons);
    float totalcosts = (totalconsumption * (this->price_kWh));
    int index = 1;

    Device* currentdevice = this->devices;

    while (currentdevice != nullptr)
    {
        totalconsumption += currentdevice->annual_kWh();
        currentdevice->print(index, this->price_kWh);
        currentdevice = currentdevice->get_next();
        index++;
    }

    cout << string(columnwidth * 2, '-') << endl;
    cout << right << setw(columnwidth) << "power consumption square meters" << ": " << left << powerconsumptionsquare_meters << " kwh" << endl;
    cout << right << setw(columnwidth) << "power consumption all persons" << ": " << left << powerconsumptionpersons << " kwh" << endl;
    cout << right << setw(columnwidth) << "total annual power consumption" << ": " << left << totalconsumption << " kwh" << endl;
    cout << right << setw(columnwidth) << "total annual power costs" << ": " << left << (totalconsumption * this->price_kWh) << " eur" << endl;
}


class Address
{
private:
    string street;
    string no;
    string zip;
    string city;
 public:
    Address() { street = no = zip = city = ""; }

    Address(string street, string no, string zip, string city)
    {
        this->street = street;
        this->no = no;
        this->zip = zip;
        this->city = city;
    }
    // getter method
    string get_street() { return this->street; }
    string get_no() { return this->no; }
    string get_zip() { return this->zip; }
    string get_city() { return this->city; }
    string to_string() { return this->street + " " + this->no + ", " + this->zip + " " + this->city; }

    void input()
    {
        cin.ignore();
        cout << " what is the street name? ";
        getline(cin, street);
        cout << "what is house number? ";
        getline(cin, no);
        cout << "what is zip code? ";
        getline(cin, zip);
        cout << "what is the city name? ";
        getline(cin, this->city);
    }

    ~Address()
    {
        cout << "Address " << this->street << ' ' << this->no << ", " << this->zip << ' ' << this->city << " at address " << this << " deleted\n";
    }
};

class House
{
 private:
    int number_of_households;
    Address address;
    Household** household;
 public:
    House(int n, Address a)
    {
        Address b(a);
        this->number_of_households = n;
        this->address = a;
        this->household = new Household * [number_of_households];
        for (int i = 0; i < number_of_households; i++)
        {
            this->household[i] = nullptr;
        }
    }
    // public member function
    Household* get_household(int n) { return this->household[n]; }
    Household* operator[](int n) { return this->household[n]; }
    Address get_address() { return address; }
    void set_household(Household* household, int n) { this->household[n] = household; }
    void write_use_to_file(ofstream& ofs, Use curr);
    static Address readcityinformation(ifstream& ifs, string& taskidentifier, char separator, int& length);
    static void readhouseholdinformation(string line, char separator, int startindex, int& index, Household& household);
    static Use readfrequencyofuse(string curr);
    static void readimmobileinformation(string line, char separator, int startindex, Immobile& immobile);
    static void readmobileinformation(string line, char separator, int startindex, Mobile& mobile);
    static void readsolarinformation(string line, char separator, int startindex, Solar& solar);
    void print_all();
    void write_data_to_file(string filename, char separator);
    static House* read_from_file(string filename, string& taskidentifier, char separator, int& length);

    ~House()
    {
        // Delete all households
        for (int i = 0; i < number_of_households; i++)
        {
            delete household[i];
        }

        cout << "House at address " << this << " deleted\n";
        delete[] household;
    }
};

void House::write_use_to_file(ofstream& ofs, Use curr) // write the frequency of use to a file
{
    if (curr == Use::once)
        ofs << "once";
    else if (curr == Use::daily)
        ofs << "daily";
    else if (curr == Use::mo_fr)
        ofs << "monday to friday";
    else if (curr == Use::sa_su)
        ofs << "saturday and sunday";
    else
        ofs << "weekly";
}

Address House::readcityinformation(ifstream& ifs, string& taskidentifier, char separator, int& length)
{
    // read a line from the input stream
    string line;
    getline(ifs, line);

    // extract task identifier
    int i = 0;
    string curr = "";
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    taskidentifier = curr;
    curr = "";
    i++;

    // extract length
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    length = stoi(curr);
    curr = "";

    string street, houseNo, zipCode, cityName;

    // extract street
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    street = curr;
    curr = "";
    i++;

    // extract house number
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    houseNo = curr;
    curr = "";
    i++;

    // extract zip code
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    zipCode = curr;
    curr = "";
    i++;

    // extract city name
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    cityName = curr;
    curr = "";
    i++;
    Address address(street, houseNo, zipCode, cityName);
    return address;

}

void House::readhouseholdinformation(string line, char separator, int startindex, int& index, Household& household)
{
    // extract index
    string curr = "";
    int i = startindex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    index = stoi(curr);
    curr = "";

    // extract hot water indicator
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    bool hotwater = ((curr == "true") ? true : false);
    household.set_electric_water_heating(hotwater);

    curr = "";

    // extract number of people
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    int numberofpeople = stoi(curr);
    household.set_persons(numberofpeople);
    curr = "";
    i++;

    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    int square_meters = stoi(curr);
    household.set_square_meters(square_meters);
    curr = "";
    i++;

    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    float price_kwh = stof(curr);
    household.set_price_kWh(price_kwh);
    curr = "";
    i++;

    while (i < line.length())
    {
        curr += line[i];
        i++;
    }

    string power_supplier = curr;
    household.set_power_supplier(power_supplier);
}

// convert a string to an enumerated type representing frequency of use
Use House::readfrequencyofuse(string curr)
{
    if (curr == "once")
        return Use::once;
    else if (curr == "daily")
        return Use::daily;
    else if (curr == "Monday to Friday")
        return Use::mo_fr;
    else if (curr == "Saturday and Sunday")
        return Use::sa_su;
    else
        return Use::weekly;
}

void House::readimmobileinformation(string line, char separator, int startindex, Immobile& consumer)
{
    // extract description
    string curr = "";
    int i = startindex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    consumer.set_description(curr);
    curr = "";

    // extract hours
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    consumer.set_hours(stof(curr));
    curr = "";

    // extract use (frequency of use)
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    Use use = readfrequencyofuse(curr);
    consumer.set_use(use);
    i++;
    curr = "";

    // extract watt
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    consumer.set_watt(stof(curr));
    i++;
    curr = "";

    // extract standby watt
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    consumer.set_watt_standby(stof(curr));
}

void House::readmobileinformation(string line, char separator, int startindex, Mobile& mobile)
{
    // extract description
    string curr = "";
    int i = startindex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    mobile.set_description(curr);
    curr = "";

    // extract km
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    mobile.set_km(stof(curr));
    curr = "";

    // extract use (frequency of use)
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    Use use = readfrequencyofuse(curr);
    mobile.set_use(use);
    i++;
    curr = "";

    // extract watt
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    mobile.set_watt(stof(curr));
}

void House::readsolarinformation(string line, char separator, int startindex, Solar& solar)
{
    // extract description
    string curr = "";
    int i = startindex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    solar.set_description(curr);
    curr = "";

    // extract year
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    solar.set_year(stoi(curr));
    curr = "";


    // extract watt_peak
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    solar.set_watt_peak(stof(curr));
}

void House::print_all()
{
    cout << " ======================================================================\n";
    cout << "                                 H O U S E\n";
    cout << " ======================================================================\n";
    cout << "                                 (this: " << this->household << '\n';
    cout << "                               address: " << this->address.to_string() << '\n';
    cout << "                  number of households: " << this->number_of_households << '\n';
    cout << "         total number of all consumers: " << Consumer::get_total_number() << '\n';


    for (int i = 0; i < this->number_of_households; i++)
    {
        if (this->household[i] != nullptr)
        {
            this->household[i]->printhousehold(i);
        }
    }
}

House* House::read_from_file(string filename, string& taskidentifier, char separator, int& length)
{
    // open the input file
    ifstream ifs(filename);
    if (!ifs.is_open())
    {
        cout << "could not open file " << filename << "!\n";
        return nullptr;
    }

    string line;
    cout << "input file " << filename << " opened...\n";
    
    // read city information from the file
    Address a = readcityinformation(ifs, taskidentifier, separator, length);
    House* house = new House(length, a);

    house->household = new Household * [length];

    for (int i = 0; i < length; i++)
    {
        house->household[i] = nullptr;
    }
    // read data for each household and consumer from the file
    string curr = "";
    int i = 0;

    int index;
    int totalConsumers = 0;
    while (getline(ifs, line))
    {
        i = 0;
        // extract the type (household or consumer)
        while (i < line.length() && line[i] != separator)
        {
            curr += line[i];
            i++;
        }
        i++;
        if (curr == "Household")
        {
            // create a new household and read its information
            Household* household = new Household;
            household->set_devices(nullptr);

            readhouseholdinformation(line, separator, i, index, *household);
            house->household[index] = household;
        }
        else if (curr == "Immobile")
        {
            Immobile* immobile = new Immobile;
            readimmobileinformation(line, separator, i, *immobile);
            house->household[index]->add_device_to_household(immobile, true);
            totalConsumers++;
        }
        else if (curr == "Mobile")
        {
            Mobile* mobile = new Mobile;
            readmobileinformation(line, separator, i, *mobile);
            house->household[index]->add_device_to_household(mobile, true);
            totalConsumers++;
        }
        else if (curr == "Solar")
        {
            Solar* solar = new Solar;
            readsolarinformation(line, separator, i, *solar);
            house->household[index]->add_device_to_household(solar, true);
            totalConsumers++;
        }
        curr = "";

    }

    // close the input file
    ifs.close();
    cout << "input file " << filename << " closed...\n";
    return house;
}

// write household and consumer data to a file
void House::write_data_to_file(string filename, char separator)
{
    // open the output file
    ofstream ofs(filename);

    cout << "output file " << filename << " opened...\n";

    // write header information to the file
    ofs << "A6" << separator << this->number_of_households << separator << address.get_street() << separator << address.get_no() << separator << address.get_zip() << separator << address.get_city() << '\n';

    // iterate through each household
    for (int i = 0; i < this->number_of_households; i++)
    {
        // check if the household exists
        if (this->household[i])
        {
            // write household information to the file
            ofs << "household" << separator << to_string(i) << separator;
            if (this->household[i]->get_electric_water_heating())
                ofs << "true";
            else
                ofs << "false";
            ofs << separator;
            ofs << to_string(this->household[i]->get_persons()) << separator << to_string(this->household[i]->get_square_meters()) << separator;
            ofs << to_string(this->household[i]->get_price_kWh()) << separator << this->household[i]->get_power_supplier() << '\n';

            // iterate through each device in the household
            Device* curr = this->household[i]->get_devices();
            while (curr)
            {
                ofs << curr->get_string_for_file(separator) << '\n';
                curr->set_next(curr->get_next());
                curr = curr->get_next();
            }
        }
    }

    // close the output file
    cout << "output file " << filename << " closed...\n";
    ofs.close();
}

// main function
int main()
{
    string filename;
    string taskidentifer;
    int length, n;

    House* house = nullptr;
    Address a;

    while (true)
    {
        cout << "q quit" << endl;
        cout << "d delete house" << endl;
        cout << "h house initialisation" << endl;
        cout << "i input immobile consumer" << endl;
        cout << "m input mobile consumer" << endl;
        cout << "s input solar producer" << endl;
        cout << "u move up device" << endl;
        cout << "p print household" << endl;
        cout << "a print all households" << endl;
        cout << "n new household" << endl;
        cout << "c copy all consumers (added to already existing ones)" << endl;
        cout << "r read data from file" << endl;
        cout << "w write data into file\n>> ";


        char choice;
        cin >> choice;

        if (choice == 'q')
        {
            break;
        }
        else if (choice == 'd')
        {
            house->~House();
            house = nullptr;
        }
        else if (choice == 'h')
        {
            int number_of_households;
            cout << "how many households does the house have? ";
            cin >> number_of_households;

            a.input();
            house = new House(number_of_households, a);
        }
        else if (choice == 'i')
        {
            int num;
            cout << "number of household?";
            cin >> num;

            Consumer* new_immobile = new Immobile();
            new_immobile->input();

            Household* h = house->get_household(num);

            h->add_device_to_household(new_immobile, false);
        }
        else if (choice == 'm')
        {
            int num;
            cout << "number of household?";
            cin >> num;

            Consumer* new_mobile = new Mobile();
            new_mobile->input();

            Household* h = house->get_household(num);
            h->add_device_to_household(new_mobile, false);
        }
        else if (choice == 's')
        {
            int num;
            cout << "number of household?";
            cin >> num;

            Producer* new_producer = new Solar;
            new_producer->input();

            Household* h = house->get_household(num);

            h->add_device_to_household(new_producer, false);
        }
        else if (choice == 'u')
        {
            int k, num;
            cout << "number of household?";
            cin >> num;
            cout << "which one? ";
            cin >> k;

            Household* h = house->get_household(num);
            h->move_up(k);
        }
        else if (choice == 'p')
        {
            int number;
            cout << "number of household";
            cin >> number;

            Household* h = house->get_household(number);
            h->printhousehold(number);
        }
        else if (choice == 'a')
        {
            if (house == nullptr)
            {
                cout << "house is a nullptr, please first choose h to initialise a new house or r to read from file\n";
            }
            else
                house->print_all();
        }

        else if (choice == 'n')
        {
            int x;
            cout << "number of household? ";
            cin >> x;

            Household* h = house->get_household(x);

            if (h != nullptr)
            {
                cout << "household already exists" << endl;
            }
            else
            {
                Household* new_household = new Household;
                new_household->input_household();
                house->set_household(new_household, x);
            }
        }
        else if (choice == 'c')
        {
            int num1, num2;
            cout << "number of household from which to copy consumers?";
            cin >> num1;
            cout << "number of household to copy to?";
            cin >> num2;
            Household* h1 = house->get_household(num1);
            Household* h2 = house->get_household(num2);

            if (h1 && h2)
            {
                h2->copy_devices(h1);
            }

        }
        else if (choice == 'r')
        {
            string filename;
            char separator;
            cout << "input file name: ";
            cin >> filename;

            cout << "input separator character: ";
            cin >> separator;

            ifstream ifs(filename);

            house = House::read_from_file(filename, taskidentifer, separator, length);
        }
        else if (choice == 'w')
        {

            string filename;
            char separator;
            cout << "output file name: ";
            cin >> filename;

            cout << "output separator character: ";
            cin >> separator;

            house->write_data_to_file(filename, separator);
        }
        else
        {
            cout << "sorry wrong choice" << endl;
        }
    }
    return 0;
}