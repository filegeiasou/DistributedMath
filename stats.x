struct input1 {
    int y<>;
};

struct input2 {
    double a;
    int y<>;
};

struct output1 {
    int min_max[2];
};

struct output2 {
    double pr_aY<>;
};

program ADD_PROG {
                version ADD_VERS {
                    double average(input1) = 1;
                    output1 min_max1(input1) = 2;
                    output2 prod_aY(input2) = 3;
        } = 1;
} = 0x23451111;