#ifndef MAIN_H
#define MAIN_H

namespace Ui {

    namespace TPatient {

        const int PATIENT_ID_INDEX = 0;
        const int FIRST_NAME_INDEX = 1;
        const int SECOND_NAME_INDEX = 2;
        const int LAST_NAME_INDEX = 3;
        const int BIRTH_DATE_INDEX = 4;
        const int STREET_INDEX = 5;
        const int BUILDING_INDEX = 6;
        const int BLOCK_INDEX = 7;
        const int APARTMENTS_INDEX = 8;
        const int PHONE_NUMBER_INDEX = 9;
        const int ANAMNESIS_INDEX = 10;
    }

    namespace TCheck {

        const int CHECK_ID_INDEX = 0;
        const int TAG_INDEX = 1;
        const int DATE_INDEX = 2;
        const int PATIENT_ID_INDEX = 3;
        const int RESOLUTION_INDEX = 4;
    }

    enum class Form_Mode {CREATE_RECORD_MODE, EDIT_RECORD_MODE};
}

#endif // MAIN_H
