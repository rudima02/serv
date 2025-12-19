#pragma once

#include <string>
#include <vector>
#include <memory>
#include <odb/core.hxx>

using namespace std;

// ==================== ВСЕ КЛАССЫ В ОДНОМ ФАЙЛЕ ====================

#pragma db object table("roles")
class Roles {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    string name_role;
public:
    Roles() = default;
    Roles(const string& name_role) : name_role(name_role) {}
    unsigned long getID() const { return id; }
    const string& getName() const { return name_role; }
};

#pragma db object table("category")
class Category {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    #pragma db column("name_category")
    string name;
public:
    Category() = default;
    Category(const string& name) : name(name) {}
    unsigned long getID() const { return id; }
    const string& getName() const { return name; }
};

#pragma db object table("po")
class PO {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    string name_po;
    #pragma db not_null
    shared_ptr<Category> category;
public:
    PO() = default;
    PO(const string& name_po, shared_ptr<Category> cat) 
        : name_po(name_po), category(cat) {}
    unsigned long getID() const { return id; }
    const string& getName() const { return name_po; }
    shared_ptr<Category> getCategory() const { return category; }
};

#pragma db object table("versionpo")
class VersionPO {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    string version;
    string date_version;
    #pragma db not_null
    shared_ptr<PO> po;
public:
    VersionPO() = default;
    VersionPO(const string& version, const string& date_version, shared_ptr<PO> po)
        : version(version), date_version(date_version), po(po) {}
    unsigned long getID() const { return id; }
    const string& getVersion() const { return version; }
    const string& getDate() const { return date_version; }
    shared_ptr<PO> getPO() const { return po; }
};

#pragma db object table("department")
class Department {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    #pragma db column("name_department")
    string name;
public:
    Department() = default;
    Department(const string& name) : name(name) {}
    unsigned long getID() const { return id; }
    const string& getName() const { return name; }
};

#pragma db object table("pc")
class PC {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    string name_pc;
    #pragma db not_null
    shared_ptr<Department> department;
public:
    PC() = default;
    PC(const string& name_pc, shared_ptr<Department> dept)
        : name_pc(name_pc), department(dept) {}
    unsigned long getID() const { return id; }
    const string& getName() const { return name_pc; }
    shared_ptr<Department> getDepartment() const { return department; }
};

#pragma db object table("semi")
class Semi {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    #pragma db not_null
    shared_ptr<PO> po;
    #pragma db not_null
    shared_ptr<PC> pc;
public:
    Semi() = default;
    Semi(shared_ptr<PO> po, shared_ptr<PC> pc) : po(po), pc(pc) {}
    unsigned long getID() const { return id; }
    shared_ptr<PO> getPO() const { return po; }
    shared_ptr<PC> getPC() const { return pc; }
};

#pragma db object table("users")
class Users {
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long id;
    string name_user;
    #pragma db not_null
    shared_ptr<Department> department;
    #pragma db not_null
    shared_ptr<Roles> roles;
public:
    Users() = default;
    Users(const string& name_user, shared_ptr<Department> dept, shared_ptr<Roles> role)
        : name_user(name_user), department(dept), roles(role) {}
    unsigned long getID() const { return id; }
    const string& getName() const { return name_user; }
    shared_ptr<Department> getDepartment() const { return department; }
    shared_ptr<Roles> getRole() const { return roles; }
};
