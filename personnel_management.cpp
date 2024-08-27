#include "./personnel_management.h"

bool PersonnelManagement::ID_in_use[MAX_PERSONNELS] = {0};

int PersonnelManagement::first_unused_ID()
{
    for (int i = 0; i < MAX_PERSONNELS; ++i)
        if (!PersonnelManagement::ID_in_use[i])
            return i;
    return -1;  // In case all ID are used
}

void PersonnelManagement::importPersonnel()
{
    std::ifstream fin("./personnel_list.txt", std::ifstream::app);
    if (fin.is_open())
    {
        PersonnelManagement::Personnel p;
        while (!fin.eof())
        {
            std::string tmp;
            getline(fin, tmp, '|');
            if (tmp[tmp.size() - 1] == '\n')
                break;
            p.ID = std::stoi(tmp);
            getline(fin, p.name, '|');
            getline(fin, p.address, '|');
            getline(fin, p.phone, '|');
            getline(fin, p.email, '|');
            getline(fin, p.department, '|');
            getline(fin, p.rank, '|');
            getline(fin, p.position, '|');
            getline(fin, p.task, '|');
            getline(fin, tmp, '|');
            p.salary = std::stoi(tmp);
            getline(fin, p.currency, '|');

            ID_in_use[p.ID] = true;

            Asset::personnelTree.insert(p);
        }
        fin.close();
    }
    else
        std::cout << "Oops, something went wrong!\n";
}

void print_line_to_file(std::ofstream& fout, PersonnelManagement::Personnel p)
{
    fout << p.ID << '|' << p.name << '|' << p.address << '|' << p.phone << '|' << p.email << '|' << p.department
        << '|' << p.rank << '|' << p.position << '|' << p.task << '|' << p.salary << '|' << p.currency << "|\n";
}

void PersonnelManagement::exportPersonnel()
{
    std::ofstream fout("./personnel_list.txt", std::ofstream::trunc);
    if (fout.is_open())
    {
        Asset::personnelTree.export_to_file(fout, print_line_to_file);
        
        fout.close();
    }
    else
        std::cout << "Oops, something went wrong!\n";
}

PersonnelManagement::PersonnelManagement()
{
    importPersonnel();
}

PersonnelManagement::~PersonnelManagement()
{
    exportPersonnel();
}

void PersonnelManagement::addPersonnel(std::string rank)
{
    if (rank != "Staff")
    {
        std::cin.ignore();
        PersonnelManagement::Personnel p;

        std::cout << "- Name: ";
        getline(std::cin, p.name);
        std::cout << "- Address: ";
        getline(std::cin, p.address);
        std::cout << "- Phone number: ";
        getline(std::cin, p.phone);
        std::cout << "- Email: ";
        getline(std::cin, p.email);
        std::cout << "- Department: ";
        getline(std::cin, p.department);
        std::cout << "- Rank (Staff/Manager/Owner): ";
        getline(std::cin, p.rank);
        while (p.rank != "Staff" && p.rank != "Manager" && p.rank != "Owner")
        {
            std::cout << "- Invalid answer! Please enter again: ";
            getline(std::cin, p.rank);
        }
        std::cout << "- Position: ";
        getline(std::cin, p.position);
        p.currency = "USD";

        p.ID = first_unused_ID();
        ID_in_use[p.ID] = true;

        Asset::personnelTree.insert(p);
    }
    else
        std::cout << "You don't have permission to do this!\n";
}

bool PersonnelManagement::removePersonnel(std::string rank)
{
    if (rank != "Staff")
    {
        int ID;
        std::cout << "Enter ID: ";
        std::cin >> ID;
        PersonnelManagement::Personnel p;
        p.ID = ID;
        if (Asset::personnelTree.find(p))
        {
            Asset::personnelTree.erase(p);
            ID_in_use[p.ID] = false;
            return true;
        }
        std::cout << "There's no personnel with this ID!\n";
        return false;
    }
    else
    {
        std::cout << "You don't have permission to do this!\n";
        return false;
    }
}

bool PersonnelManagement::updatePersonnel(std::string rank)
{
    if (rank != "Staff")
    {
        int ID;
        std::cout << "Enter ID: ";
        std::cin >> ID;
        std::cin.ignore();
        PersonnelManagement::Personnel p;
        p.ID = ID;
        if (Asset::personnelTree.find(p))
        {
            std::cout << "This personnel's current information:";
            std::cout << "\n- Name: " << p.name;
            std::cout << "\n- Address: " << p.address;
            std::cout << "\n- Phone number: " << p.phone;
            std::cout << "\n- Email: " << p.email;
            std::cout << "\n- Department: " << p.department;
            std::cout << "\n- Rank: " << p.rank;
            std::cout << "\n- Position: " << p.position << "\n\n";
            
            std::string answer;
            std::cout << "Update this personnel's information (enter \"keep\" if there's no changes):\n";
            std::cout << "- Name: ";
            getline(std::cin, answer);
            if (answer != "keep")
                p.name = answer;
            std::cout << "- Address: ";
            getline(std::cin, answer);
            if (answer != "keep")
                p.address = answer;
            std::cout << "- Phone number: ";
            getline(std::cin, answer);
            if (answer != "keep")
                p.phone = answer;
            std::cout << "- Email: ";
            getline(std::cin, answer);
            if (answer != "keep")
                p.email = answer;
            std::cout << "- Department: ";
            getline(std::cin, answer);
            if (answer != "keep")
                p.department = answer;
            std::cout << "- Rank: ";
            getline(std::cin, answer);
            if (answer != "keep")
                p.rank = answer;
            std::cout << "- Position: ";
            getline(std::cin, answer);
            if (answer != "keep")
                p.position = answer;

            Asset::personnelTree.update(p);
            return true;
        }
        std::cout << "There's no personnel with this ID!\n";
        return false;
    }
    else
    {
        std::cout << "You don't have permission to do this!\n";
        return false;
    }
}

void PersonnelManagement::listPersonnel(std::string rank)
{
    if (rank != "Staff")
    {
        std::cout << "\n\t\t\t\tLIST OF PERSONNELS\n\n";
        int w = ID_W + NAME_W + DEPT_W + RANK_W + POS_W + TASK_W;

        std::cout << std::right << std::setw(w) << std::setfill('=') << '\n';
        std::cout << std::left << std::setfill(' ');
        std::cout << std::setw(ID_W) << "ID" << std::setw(NAME_W) << "NAME" << std::setw(DEPT_W) << "DEPARTMENT"
                << std::setw(RANK_W) << "RANK" << std::setw(POS_W) << "POSITION" << std::setw(TASK_W) << "TASK" << '\n';
        std::cout << std::right << std::setw(w) << std::setfill('-') << '\n';

        Asset::personnelTree.print();

        std::cout << std::right << std::setw(w) << std::setfill('=') << '\n';
        std::cout << std::left << std::setfill(' ');
    }
    else
        std::cout << "You don't have permission to do this!\n";
}

bool PersonnelManagement::searchPersonnel(std::string rank)
{
    if (rank != "Staff")
    {
        int ID;
        std::cout << "Enter ID: ";
        std::cin >> ID;
        PersonnelManagement::Personnel p;
        p.ID = ID;
        if (Asset::personnelTree.find(p))
        {
            std::cout << "Information:";
            std::cout << "\n- Name: " << p.name;
            std::cout << "\n- Address: " << p.address;
            std::cout << "\n- Phone number: " << p.phone;
            std::cout << "\n- Email: " << p.email;
            std::cout << "\n- Department: " << p.department;
            std::cout << "\n- Rank: " << p.rank;
            std::cout << "\n- Position: " << p.position;
            std::cout << "\n- Task: " << p.task << '\n';
            return true;
        }
        else
        {
            std::cout << "There's no personnel with this ID!\n";
            return false;
        }
    }
    else
    {
        std::cout << "You don't have permission to do this!\n";
        return false;
    }
}

void PersonnelManagement::assignTask(std::string rank)
{
    if (rank != "Staff")
    {
        int ID;
        std::cout << "Enter ID: ";
        std::cin >> ID;
        std::cin.ignore();
        PersonnelManagement::Personnel p;
        p.ID = ID;
        if (Asset::personnelTree.find(p))
        {
            std::cout << "Information:";
            std::cout << "\n- Name: " << p.name;
            std::cout << "\n- Address: " << p.address;
            std::cout << "\n- Phone number: " << p.phone;
            std::cout << "\n- Email: " << p.email;
            std::cout << "\n- Department: " << p.department;
            std::cout << "\n- Rank: " << p.rank;
            std::cout << "\n- Position: " << p.position;
            std::cout << "\nAssign task: ";
            getline(std::cin, p.task);
            
            Asset::personnelTree.update(p);
        }
        else
            std::cout << "There's no personnel with this ID!\n";
    }
    else
        std::cout << "You don't have permission to do this!\n";
}

void PersonnelManagement::listTask()
{
    std::ifstream fin("./task_list.txt");
    if (fin.is_open())
    {
        std::cout << "\n\t\t\tLIST OF TASKS\n\n";

        int w = RANK_W + POS_W + TASK_W;
        std::cout << std::right << std::setw(w) << std::setfill('=') << '\n';
        std::cout << std::left << std::setfill(' ');
        std::cout << std::setw(RANK_W) << "RANK" << std::setw(POS_W) << "POSITION" << std::setw(TASK_W) << "TASK (in general)" << '\n';
        std::cout << std::right << std::setw(w) << std::setfill('-') << '\n';
        
        std::cout << std::left << std::setfill(' ');
        while (!fin.eof())
        {
            std::string rank, position, task;
            getline(fin, rank, ',');
            getline(fin, position, ',');
            getline(fin, task);
            std::cout << std::setw(RANK_W) << rank << std::setw(POS_W) << position << std::setw(TASK_W) << task << '\n';
        }

        std::cout << std::right << std::setw(w) << std::setfill('=') << '\n';
        std::cout << std::left << std::setfill(' ');

        fin.close();
    }
    else
        std::cout << "Oops, something went wrong!\n";
}

void PersonnelManagement::stepbystepForPersonnelManagement(int ID, std::string rank)
{
    while (true)
    {
        std::cout << "\t\t\tPERSONNEL MANAGEMENT\n";
        std::cout << "1. Add a personnel\n";
        std::cout << "2. Remove a personnel\n";
        std::cout << "3. Update personnel\n";
        std::cout << "4. Print a list of personnels\n";
        std::cout << "5. Search for a personnel\n";
        std::cout << "6. Assign task for a personnel\n";
        std::cout << "7. List all tasks in the system\n";
        std::cout << "8. Exit\n\n";
        std::cout << "Your choice: ";
        int choice;
        std::cin >> choice;
        while (choice < 1 || choice > 8)
        {
            std::cout << "Invalid choice! Please enter again: ";
            std::cin >> choice;
        }

        bool is_successful = 0;
        if (choice == 1)
            addPersonnel(rank);
        else if (choice == 2)
            is_successful = removePersonnel(rank);
        else if (choice == 3)
            is_successful = updatePersonnel(rank);
        else if (choice == 4)
            listPersonnel(rank);
        else if (choice == 5)
            is_successful = searchPersonnel(rank);
        else if (choice == 6)
            assignTask(rank);
        else if (choice == 7)
            listTask();
        else
            break;
    }
}
