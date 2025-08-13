std::string get_method_modifier(uint16_t flags) {
    std::stringstream outPut;
    auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;
    switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE:
            outPut << "private ";
            break;
        case METHOD_ATTRIBUTE_PUBLIC:
            outPut << "public ";
            break;
        case METHOD_ATTRIBUTE_FAMILY:
            outPut << "protected ";
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
            outPut << "internal ";
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & METHOD_ATTRIBUTE_STATIC) {
        outPut << "static ";
    }
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "sealed override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            outPut << "virtual ";
        } else {
            outPut << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
        outPut << "extern ";
    }
    return outPut.str();
}

std::string dump_method(Il2CppClass * klass) {
    std::stringstream outPut;
    if (klass->method_count > 0) {
        outPut << "\n\t// Methods\n";
        void *iter = nullptr;
        while (auto method = il2cpp_class_get_methods(klass, &iter)) {
            //TODO attribute
            if (method->methodPointer) {
                outPut << "\t// RVA: 0x";
                std::stringstream hhk;
                hhk << std::hex << (uint64_t) method->methodPointer - il2cpp_base_z;
                std::string upr = hhk.str();
                for (auto & c: upr) c = toupper(c);
                outPut << upr;
            } else {
                outPut << "\t// RVA: 0x0";
            }
            if (method->slot != 65535) {
                outPut << " Slot: " << std::dec << method->slot;
            }
            outPut << "\n\t";
            outPut << get_method_modifier(method->flags);
            //TODO genericContainerIndex
            auto return_type = method->return_type;
            if (return_type->byref) {
                outPut << "ref ";
            }
            auto return_class = il2cpp_class_from_type(return_type);
            outPut << return_class->name << " " << method->name << "(";
            for (int i = 0; i < method->parameters_count; ++i) {
                auto parameters = method->parameters[i];
                auto parameter_type = parameters;
                auto attrs = parameter_type->attrs;
                if (parameter_type->byref) {
                    if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) {
                        outPut << "out ";
                    } else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) {
                        outPut << "in ";
                    } else {
                        outPut << "ref ";
                    }
                } else {
                    if (attrs & PARAM_ATTRIBUTE_IN) {
                        outPut << "[In] ";
                    }
                    if (attrs & PARAM_ATTRIBUTE_OUT) {
                        outPut << "[Out] ";
                    }
                }
                auto parameter_class = il2cpp_class_from_type(parameter_type);
                outPut << parameter_class->name << " " << "param" << std::to_string((i+1));
                outPut << ", ";
            }
            if (method->parameters_count > 0) {
                outPut.seekp(-2, outPut.cur);
            }
            outPut << ") { }\n\n";
            //TODO GenericInstMethod
        }
    }
    return outPut.str();
}

std::string dump_field(Il2CppClass * klass) {
    std::stringstream outPut;
    if (klass->field_count > 0) {
        outPut << "\n\t// Fields\n";
        void *iter = nullptr;
        while (auto field = il2cpp_class_get_fields(klass, &iter)) {
            //TODO attribute
            outPut << "\t";
            auto attrs = field->type->attrs;
            auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
            switch (access) {
                case FIELD_ATTRIBUTE_PRIVATE:
                    outPut << "private ";
                    break;
                case FIELD_ATTRIBUTE_PUBLIC:
                    outPut << "public ";
                    break;
                case FIELD_ATTRIBUTE_FAMILY:
                    outPut << "protected ";
                    break;
                case FIELD_ATTRIBUTE_ASSEMBLY:
                case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                    outPut << "internal ";
                    break;
                case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                    outPut << "protected internal ";
                    break;
            }
            if (attrs & FIELD_ATTRIBUTE_LITERAL) {
                outPut << "const ";
            } else {
                if (attrs & FIELD_ATTRIBUTE_STATIC) {
                    outPut << "static ";
                }
                if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                    outPut << "readonly ";
                }
            }
            auto field_class = il2cpp_class_from_type(field->type);
            outPut << field_class->name << " " << field->name;
            outPut << "; // 0x" << std::hex << field->offset << "\n";
        }
    }
    return outPut.str();
}

std::string dump_property(Il2CppClass * klass) {
    std::stringstream outPut;
    if (klass->property_count > 0) {
        outPut << "\n\t// Properties\n";
        void *iter = nullptr;
        while (auto prop = il2cpp_class_get_properties(klass, &iter)) {
            //TODO attribute
            outPut << "\t";
            Il2CppClass * prop_class = nullptr;
            if (prop->get) {
                outPut << get_method_modifier(prop->get->flags);
                prop_class = il2cpp_class_from_type(prop->get->return_type);
            } else if (prop->set) {
                outPut << get_method_modifier(prop->set->flags);
                prop_class = il2cpp_class_from_type(prop->set->parameters[0]);
            }
            outPut << prop_class->name << " " << prop->name << " { ";
            if (prop->get) {
                outPut << "get; ";
            }
            if (prop->set) {
                outPut << "set; ";
            }
            outPut << "}\n";
        }
    }
    return outPut.str();
}

std::string dump_type(Il2CppClass* klass, int klassIndex) {
    std::stringstream outPut;
    if (strstr(klass->namespaze, "PrivateImplement")) return "";
    outPut << "\n// Namespace: " << klass->namespaze << "\n";
    auto flags = il2cpp_class_get_flags(klass);
    if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
        outPut << "[Serializable]\n";
    }
    //TODO attribute
    //outPut << "public ";
    auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
    switch (visibility) {
        case TYPE_ATTRIBUTE_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_PUBLIC:
            outPut << "public ";
            break;
        case TYPE_ATTRIBUTE_NOT_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
        case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
            outPut << "internal ";
            break;
        case TYPE_ATTRIBUTE_NESTED_PRIVATE:
            outPut << "private ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAMILY:
            outPut << "protected ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "static ";
    } else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
    } else if (!klass->valuetype && !klass->enumtype && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "sealed ";
    }
    if (flags & TYPE_ATTRIBUTE_INTERFACE) {
        outPut << "interface ";
    } else if (klass->enumtype) {
        outPut << "enum ";
    } else if (klass->valuetype) {
        outPut << "struct ";
    } else {
        outPut << "class ";
    }
    //outPut << "class ";
    outPut << klass->name; //TODO genericContainerIndex
    if (strstr(klass->name, "ArrayCopy") || strstr(klass->name, "ArrayType") || strstr(klass->name, "OBFUSCATOR")) return "";
    std::vector<std::string> extends;
    if (!klass->valuetype && !klass->enumtype && klass->parent) {
        auto parent_type = il2cpp_class_get_type(klass->parent);
        if (parent_type->type != IL2CPP_TYPE_OBJECT) {
            extends.emplace_back(klass->parent->name);
        }
    }
    if (klass->interfaces_count > 0) {
        void *iter = nullptr;
        while (auto itf = il2cpp_class_get_interfaces(klass, &iter)) {
            extends.emplace_back(itf->name);
        }
    }
    if (!extends.empty()) {
        outPut << " : " << extends[0];
        for (int i = 1; i < extends.size(); ++i) {
            outPut << ", " << extends[i];
        }
    }
    outPut << " // TypeDefIndex: " << klassIndex << "\n{";
    outPut << dump_field(klass);
    outPut << dump_property(klass);
    outPut << dump_method(klass);
    //TODO EventInfo
    outPut << "}\n";
    return outPut.str();
}

static int callback_z(struct dl_phdr_info *info, size_t size, void *arg) {
  (void)size, (void)arg;
  if (strstr(info->dlpi_name, "libil2cpp.so")) il2cpp_base_z = (uintptr_t)info->dlpi_addr;
  return 0;
}

#define _(jso) OBFUSCATE(jso)
