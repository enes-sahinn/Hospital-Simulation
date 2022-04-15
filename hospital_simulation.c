#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

// The number of registration desks that are available.
int REGISTRATION_SIZE = 10;
// The number of restrooms that are available.
int RESTROOM_SIZE = 10;
// The number of cashiers in cafe that are available.
int CAFE_NUMBER = 10;
// The number of General Practitioner that are available.
int GP_NUMBER = 10;
// The number of cashiers in pharmacy that are available.
int PHARMACY_NUMBER = 10;
// The number of assistants in blood lab that are available.
int BLOOD_LAB_NUMBER = 10;
// The number of operating rooms, surgeons and nurses that are available.
int OR_NUMBER = 10;
int SURGEON_NUMBER = 30;
int NURSE_NUMBER = 30;
// The maximum number of surgeons and nurses that can do a surgery. A random value is calculated
// for each operation between 1 and given values to determine the required number of surgeons and nurses.
int SURGEON_LIMIT = 5;
int NURSE_LIMIT = 5;
// The number of patients that will be generated over the course of this program.
int PATIENT_NUMBER = 1000;
// The account of hospital where the money acquired from patients are stored.
int HOSPITAL_WALLET = 0;

int WAIT_TIME = 100;
int REGISTRATION_TIME = 100;
int GP_TIME = 200;
int PHARMACY_TIME = 100;
int BLOOD_LAB_TIME = 200;
int SURGERY_TIME = 500;
int CAFE_TIME = 100;
int RESTROOM_TIME = 100;
int ARRIVAL_TIME = 75;

int REGISTRATION_COST = 100;
int PHARMACY_COST = 200; // Calculated randomly between 1 and given value.
int BLOOD_LAB_COST = 200;
int SURGERY_OR_COST = 200;
int SURGERY_SURGEON_COST = 100;
int SURGERY_NURSE_COST = 50;
int CAFE_COST = 200; // Calculated randomly between 1 and given value.

int HUNGER_INCREASE_RATE = 10;
int RESTROOM_INCREASE_RATE = 10;
pthread_mutex_t mutex;
sem_t semaphore1, semaphore2, semaphore3, semaphore4, semaphore5, semaphore6;
char med_diseases[5][15] = {"colds", "flu", "pain", "migraine", "measles"};
char surg_diseases[5][15] = {"cancer", "AIDS", "heart disease", "asthma", "mumps"};

struct Person
{
    int id;
    int Hunger_Meter;   // Initialized between 1 and 100 at creation.
    int Restroom_Meter; // Initialized between 1 and 100 at creation.
    char disease[15];
    pthread_t thread;
};

void *restroom(); // Empty function to use it before defining

void *cafe(void *patient)
{
    struct Person *p = (struct Person *)patient;
    while (1) // Infinity loop for queue
    {
        int sem5_value;
        sem_getvalue(&semaphore5, &sem5_value);
        if (sem5_value != 0) // Condition to check the availability of the queue
            break;
        usleep((rand() % WAIT_TIME + 1) * 1000);

        p->Restroom_Meter += rand() % RESTROOM_INCREASE_RATE + 1; // Restroom checking
        if (p->Restroom_Meter >= 100)
            restroom(patient);
    }
    sem_wait(&semaphore5);
    HOSPITAL_WALLET += rand() % CAFE_COST + 1;
    int cafeTime = rand() % CAFE_TIME + 1;
    usleep(cafeTime * 1000);
    printf("(%d) ate his/her food in %d milliseconds.\n", p->id, cafeTime);
    sem_post(&semaphore5);
    p->Hunger_Meter = 0; // Reset hunger meter after eating
}

void *restroom(void *patient)
{
    struct Person *p = (struct Person *)patient;
    while (1) // Infinity loop for queue
    {
        int sem6_value;
        sem_getvalue(&semaphore6, &sem6_value);
        if (sem6_value != 0) // Condition to check the availability of the queue
            break;
        printf("(%d) waiting in the restroom queue.\n", p->id);
        usleep((rand() % WAIT_TIME + 1) * 1000);

        p->Hunger_Meter += rand() % HUNGER_INCREASE_RATE + 1; // Hunger checking
        if (p->Hunger_Meter >= 100)
            cafe(patient);
    }
    sem_wait(&semaphore6);
    int restTime = rand() % RESTROOM_TIME + 1;
    usleep(restTime * 1000);
    printf("(%d) stayed in the restroom in %d milliseconds.\n", p->id, restTime);
    sem_post(&semaphore6);
    p->Restroom_Meter = 0; // Reset restroom meter after resting
}

void *registering(void *patient)
{
    struct Person *p = (struct Person *)patient;
    while (1) // Infinity loop for queue
    {
        int sem1_value;
        sem_getvalue(&semaphore1, &sem1_value);
        if (sem1_value != 0) // Condition to check the availability of the queue
            break;
        printf("(%d) waiting in the register queue.\n", p->id);
        usleep((rand() % WAIT_TIME + 1) * 1000);

        p->Hunger_Meter += rand() % HUNGER_INCREASE_RATE + 1;
        p->Restroom_Meter += rand() % RESTROOM_INCREASE_RATE + 1;
        if (p->Hunger_Meter >= 100) // Hunger checking
            cafe(patient);
        else if (p->Restroom_Meter >= 100) // Restroom checking
            restroom(patient);
    }
    sem_wait(&semaphore1);
    HOSPITAL_WALLET += REGISTRATION_COST;
    int registTime = rand() % REGISTRATION_TIME + 1;
    usleep(registTime * 1000);
    printf("(%d) was registered in %d milliseconds.\n", p->id, registTime);
    sem_post(&semaphore1);
}

void *pharmacy(void *patient)
{
    struct Person *p = (struct Person *)patient;
    while (1) // Infinity loop for queue
    {
        int sem3_value;
        sem_getvalue(&semaphore3, &sem3_value);
        if (sem3_value != 0) // Condition to check the availability of the queue
            break;
        printf("(%d) waiting in the pharmacy queue.\n", p->id);
        usleep((rand() % WAIT_TIME + 1) * 1000);
        p->Hunger_Meter += rand() % HUNGER_INCREASE_RATE + 1;
        p->Restroom_Meter += rand() % RESTROOM_INCREASE_RATE + 1;
        if (p->Hunger_Meter >= 100) // Hunger checking
            cafe(patient);
        else if (p->Restroom_Meter >= 100) // Restroom checking
            restroom(patient);
    }
    sem_wait(&semaphore3);
    HOSPITAL_WALLET += rand() % PHARMACY_COST + 1;
    int pharmacyTime = rand() % PHARMACY_TIME + 1;
    usleep(pharmacyTime * 1000);
    printf("(%d) bought the drugs in %d milliseconds.\n", p->id, pharmacyTime);
    sem_post(&semaphore3);
}

void *blood_test(void *patient)
{
    struct Person *p = (struct Person *)patient;
    while (1) // Infinity loop for queue
    {
        int sem4_value;
        sem_getvalue(&semaphore4, &sem4_value);
        if (sem4_value != 0) // Condition to check the availability of the queue
            break;
        printf("(%d) waiting in the blood_test queue.\n", p->id);
        usleep((rand() % WAIT_TIME + 1) * 1000);

        p->Hunger_Meter += rand() % HUNGER_INCREASE_RATE + 1;
        p->Restroom_Meter += rand() % RESTROOM_INCREASE_RATE + 1;
        if (p->Hunger_Meter >= 100) // Hunger checking
            cafe(patient);
        else if (p->Restroom_Meter >= 100) // Restroom checking
            restroom(patient);
    }
    sem_wait(&semaphore4);
    HOSPITAL_WALLET += rand() % BLOOD_LAB_COST + 1;
    int bloodTime = rand() % BLOOD_LAB_TIME + 1;
    usleep(bloodTime * 1000);
    printf("(%d) had a blood test in %d milliseconds.\n", p->id, bloodTime);
    sem_post(&semaphore4);
}

void *surgery(void *patient)
{
    struct Person *p = (struct Person *)patient;
    int nurse_num = rand() % NURSE_LIMIT + 1;
    int surgeon_num = rand() % SURGEON_LIMIT + 1;

    while (1) // Infinity loop for queue
    {
        if (NURSE_NUMBER >= nurse_num && SURGEON_NUMBER >= surgeon_num && OR_NUMBER >= 1) // Condition to check the availability of the queue
            break;
        usleep((rand() % WAIT_TIME + 1) * 1000);

        p->Hunger_Meter += rand() % HUNGER_INCREASE_RATE + 1;
        p->Restroom_Meter += rand() % RESTROOM_INCREASE_RATE + 1;
        if (p->Hunger_Meter >= 100) // Hunger checking
            cafe(patient);
        else if (p->Restroom_Meter >= 100) // Restroom checking
            restroom(patient);
    }
    NURSE_NUMBER -= nurse_num; // Increase variables
    SURGEON_NUMBER -= surgeon_num;
    OR_NUMBER--;

    int surgery_cost = SURGERY_OR_COST + (surgeon_num * SURGERY_SURGEON_COST) + (nurse_num * SURGERY_NURSE_COST); // Calculate surgery cost
    int surgeryTime = rand() % SURGERY_TIME + 1;
    usleep(surgeryTime * 1000);
    pthread_mutex_lock(&mutex); // I used mutex only here
    HOSPITAL_WALLET += surgery_cost;
    pthread_mutex_unlock(&mutex);
    printf("(%d) had surgery with %d surgeon, %d nurse in %d milliseconds.\n", p->id, surgeon_num, nurse_num, surgeryTime);

    NURSE_NUMBER += nurse_num; // Decrease variables
    SURGEON_NUMBER += surgeon_num;
    OR_NUMBER++;
}

void *gp_examine2(void *patient) // I used second gp examine function because this can be change dynamically
{
    struct Person *p = (struct Person *)patient;
    while (1) // Infinity looğ for queue
    {
        int sem2_value;
        sem_getvalue(&semaphore2, &sem2_value);
        if (sem2_value != 0) // Condition to check the availability of the queue
            break;
        printf("(%d) waiting in the examination queue\n", p->id);
        usleep((rand() % WAIT_TIME + 1) * 1000);

        p->Hunger_Meter += rand() % HUNGER_INCREASE_RATE + 1;
        p->Restroom_Meter += rand() % RESTROOM_INCREASE_RATE + 1;
        if (p->Hunger_Meter >= 100) // Hunger checking
            cafe(patient);
        else if (p->Restroom_Meter >= 100) // Restroom checking
            restroom(patient);
    }
    sem_wait(&semaphore2);
    int gpTime = rand() % GP_TIME + 1;
    usleep(gpTime * 1000);
    printf("(%d) was examined in %d milliseconds.\n", p->id, gpTime);
    sem_post(&semaphore2);

    if (random() > RAND_MAX / 2) // Provide %50 probability
        pharmacy(patient);       // Go to pharmacy
    else
        printf("(%d) leaving the hospital.\n", p->id);
}

void *gp_examine(void *patient)
{
    struct Person *p = (struct Person *)patient;
    while (1) // Infinity loop for queue
    {
        int sem2_value;
        sem_getvalue(&semaphore2, &sem2_value);
        if (sem2_value != 0) // Condition to check the availability of the queue
            break;
        printf("(%d) waiting in the examination queue\n", p->id);
        usleep((rand() % WAIT_TIME + 1) * 1000);

        p->Hunger_Meter += rand() % HUNGER_INCREASE_RATE + 1;
        p->Restroom_Meter += rand() % RESTROOM_INCREASE_RATE + 1;
        if (p->Hunger_Meter >= 100) // Hunger checking
            cafe(patient);
        else if (p->Restroom_Meter >= 100) // Restroom checking
            restroom(patient);
    }
    sem_wait(&semaphore2);
    int gpTime = rand() % GP_TIME + 1;
    usleep(gpTime * 1000);
    printf("(%d) was examined in %d milliseconds.\n", p->id, gpTime);
    sem_post(&semaphore2);

    int len = sizeof(med_diseases) / sizeof(med_diseases[0]); // Compute the length of the disease array
    int isMedOrBlood = 0;                                     // Boolean flag
    for (int i = 0; i < len; i++)
    {
        if (!strcmp(med_diseases[i], p->disease)) // Check if the string is in the array
        {
            isMedOrBlood = 1;
            break;
        }
    }
    if (isMedOrBlood)
    {
        if (random() > RAND_MAX / 2) // Provide %50 probability
        {
            pharmacy(patient); // Go to pharmacy
        }
        else
        {
            blood_test(patient);  // Go to blood lab
            gp_examine2(patient); // Go to gp doctor
        }
    }
    else
    {
        if (random() > RAND_MAX / 2) // Provide %50 probability
        {
            surgery(patient);     // Go to surgery
            gp_examine2(patient); // Go to gp doctor
        }
        else
        {
            blood_test(patient);  // GO to blood lab
            gp_examine2(patient); // Go to gp doctor
        }
    }
}

void *routine(void *patient)
{
    registering(patient);
    gp_examine(patient);
}

int main(int argc, char const *argv[])
{
    struct Person patients[PATIENT_NUMBER]; // Patient array
    srand(time(NULL));

    for (int i = 0; i < PATIENT_NUMBER; i++)
    {
        int med_or_surg = rand() % 2 + 1; // medicine disease or surgery disease
        int dis_index = rand() % 5;       // index of disease in the array
        if (med_or_surg == 1)
        {
            strcpy(patients[i].disease, med_diseases[dis_index]);
        }
        else
        {
            strcpy(patients[i].disease, surg_diseases[dis_index]);
        }
    }
    sem_init(&semaphore1, 0, REGISTRATION_SIZE); // Initilization of semaphores and mutex
    sem_init(&semaphore2, 0, GP_NUMBER);
    sem_init(&semaphore3, 0, PHARMACY_NUMBER);
    sem_init(&semaphore4, 0, BLOOD_LAB_NUMBER);
    sem_init(&semaphore5, 0, CAFE_NUMBER);
    sem_init(&semaphore6, 0, RESTROOM_SIZE);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < PATIENT_NUMBER; i++)
    {
        patients[i].id = i;
        patients[i].Hunger_Meter = rand() % 100 + 1;
        patients[i].Restroom_Meter = rand() % 100 + 1;

        if (pthread_create(&patients[i].thread, NULL, &routine, &patients[i]) != 0)
        {
            perror("Failed to create thread");
        }
        usleep((rand() % ARRIVAL_TIME + 1) * 1000);
    }
    for (int i = 0; i < PATIENT_NUMBER; i++)
    {
        if (pthread_join(patients[i].thread, NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);
    sem_destroy(&semaphore3);
    sem_destroy(&semaphore4);
    sem_destroy(&semaphore5);
    sem_destroy(&semaphore6);
    pthread_mutex_destroy(&mutex);

    printf("Latest status of hospital wallet: %d$\n", HOSPITAL_WALLET);
    return 0;
}