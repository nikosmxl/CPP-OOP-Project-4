
class section;
class node;

class vehicle{
    private:
        int section_index;
        bool ready;
        int destination;
        int node_index;
    public:
        vehicle(int index, int dest);

        ~vehicle();

        bool is_ready() const;

        int get_destination() const;

        void get_ready();

        void get_not_ready();

        void set_index(int index);
};

class toll{
    static const int MAX_VEHS_INSERT_IN_TOLL = 10;
    
    protected:
        int num_of_vehicles;
        int capacity;
        int index;
        static int nsegs;
        vehicle** vehicles; 

        void my_realloc(void*** m,int oldsize,int newsize);

    public:
        toll(int index_given, int nsegs_given);

        ~toll();

        void fix_matrix_reduce();

        void operate();

        int give_num_of_vehs() const;

        vehicle* give_a_veh(int& placed, int k);

};
class electronic_toll : public toll{
    
    public:
        electronic_toll(int index_given, int nsegs_given);

};
class collector_toll : public toll{

    public:
        collector_toll(int index_given, int nsegs_given);

};


class node{
    private:
        bool delays;                       // For delay messages.
        char* name_id;
        int k;
        int num_of_electronics;
        int num_of_collectors;
        toll** tolls;
        int current_toll_with_priority;
        static const short max_tolls_category = 5;

        int give_num_of_vehs() const;

    public:
        node(const char* name,int kk, int index, int nsegs);

        ~node();

        bool give_delays() const;

        vehicle** give_vehs_to_the_section(int remaining_vehs_to_full,int& vehs_to_enter);

        void operate();
};

class section{
    protected:
        bool delays;                            // For delay messages.
        int index;
        int num_of_vehicles;
        static int percent;
        int capacity_veh;
        static int nsegs;
        node* insertion;
        section* next_section;
        section* previous_section;
        vehicle** vehicles;
        
        void pass();

        void take_vehicle(vehicle* v);

        void shift(void** m,int msize);

    public:
        section(int per, int K, int capacity, section* next,int index_given, int segsnum);

        ~section();

        void set_previous_section(section* prev);

        int get_no_of_vehicles() const;

        void exit();

        void enter();

        void operate();

};

class highway{
    private:
        int num_of_vehicles;
        const int nsegs;
        section** sections;
    public:
        highway(int N, int K, int percent, int* cap);

        ~highway();

        void operate();

};