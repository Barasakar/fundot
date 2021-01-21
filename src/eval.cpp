#include <boost/dll.hpp>
#include <filesystem>
#include <sstream>

#include "fundot/fundot.h"

namespace fundot {

class Evaluator::Impl {
public:
    Object* get(Vector& owner, const Integer& integer);

    Object* get(Vector& owner, const Object& index);

    Object* get(UnorderedSet& owner, const Object& index);

    Object* get(List& owner, const Integer& integer);

    Object* get(List& owner, const Object& index);

    Object* get(Object& owner, const Getter& getter);

    Object* get(Object& owner, const Object& index);

    void set(Vector& owner, const Integer& integer, const Object& value);

    void set(Vector& owner, const Object& index, const Object& value);

    void set(UnorderedSet& owner, const Object& index, const Object& value);

    void set(List& owner, const Integer& index, const Object& value);

    void set(List& owner, const Object& index, const Object& value);

    void set(Object& owner, const Getter& index, const Object& value);

    void set(Object& owner, const Object& index, const Object& value);

    Object cond_(const List& list);

    Object eval_(const List& list);

    Object global_();

    Object if_(const List& list);

    Object import_(const List& list);

    Object let_(const List& list);

    Object local_();

    Object locate_(const List& list);

    Object while_(const List& list);

    Object eval(const Adder& adder);

    Object eval(const Subtractor& subtractor);

    Object eval(const Multiplier& multiplier);

    Object eval(const Divisor& divisor);

    Object eval(const Modular& modular);

    Object eval(const Negator& negator);

    Object eval(const Less& less);

    Object eval(const Greater& greater);

    Object eval(const LessEqual& less_equal);

    Object eval(const GreaterEqual& greater_equal);

    Object eval(const EqualTo& equal_to);

    Object eval(const NotEqualTo& not_equal_to);

    Object eval(const And& logical_and);

    Object eval(const Or& logical_or);

    Object eval(const Not& logical_not);

    Object eval(const BitwiseAnd& bitwise_and);

    Object eval(const BitwiseOr& bitwise_or);

    Object eval(const BitwiseXor& bitwise_xor);

    Object eval(const BitwiseNot& bitwise_not);

    Object eval(const LeftShift& left_shift);

    Object eval(const RightShift& right_shift);

    Object eval(const Symbol& symbol);

    Object eval(const Getter& getter);

    Object eval(const Setter& setter);

    Object eval(const Assignment& assignment);

    Object eval(const List& list);

    Object eval(const Quote& quote);

    Object eval(const SharedObject& shared_object);

    Object eval(const Object& object);

    Object eval(const std::string& script);

private:
    Object global_scope_ = {UnorderedSet()};

    Object local_scope_ = {UnorderedSet()};
};

Object* Evaluator::Impl::get(Vector& owner, const Integer& integer)
{
    if (static_cast<std::size_t>(integer.value) < owner.value.size()) {
        return &owner.value[integer.value];
    }
    return nullptr;
}

Object* Evaluator::Impl::get(Vector& owner, const Object& index)
{
    if (index.value.type() == typeid(Integer)) {
        return get(owner, std::any_cast<const Integer&>(index.value));
    }
    return nullptr;
}

Object* Evaluator::Impl::get(UnorderedSet& owner, const Object& index)
{
    auto iter = owner.value.find(index);
    if (iter != owner.value.end()) {
        if (iter->value.type() == typeid(Setter)) {
            return &const_cast<Object&>(
                std::any_cast<const Setter&>(iter->value).value.second);
        }
        return &const_cast<Object&>(*iter);
    }
    return nullptr;
}

Object* Evaluator::Impl::get(List& owner, const Integer& integer)
{
    if (static_cast<std::size_t>(integer.value) < owner.value.size()) {
        auto iter = owner.value.begin();
        std::advance(iter, integer.value);
        return &*iter;
    }
    return nullptr;
}

Object* Evaluator::Impl::get(List& owner, const Object& index)
{
    if (index.value.type() == typeid(Integer)) {
        return get(owner, std::any_cast<const Integer&>(index.value));
    }
    return nullptr;
}

Object* Evaluator::Impl::get(Object& owner, const Getter& getter)
{
    Object* obj_ptr = get(owner, getter.value.first);
    if (obj_ptr != nullptr) {
        return get(*obj_ptr, eval(getter.value.second));
    }
    return nullptr;
}

Object* Evaluator::Impl::get(Object& owner, const Object& index)
{
    if (index.value.type() == typeid(Getter)) {
        return get(owner, std::any_cast<const Getter&>(index.value));
    }
    if (owner.value.type() == typeid(UnorderedSet)) {
        return get(std::any_cast<UnorderedSet&>(owner.value), index);
    }
    if (owner.value.type() == typeid(Vector)) {
        return get(std::any_cast<Vector&>(owner.value), index);
    }
    if (owner.value.type() == typeid(List)) {
        return get(std::any_cast<List&>(owner.value), index);
    }
    return nullptr;
}

void Evaluator::Impl::set(Vector& owner, const Integer& integer,
                          const Object& value)
{
    if (static_cast<std::size_t>(integer.value) < owner.value.size()) {
        owner.value[integer.value] = value;
    }
}

void Evaluator::Impl::set(Vector& owner, const Object& index,
                          const Object& value)
{
    if (index.value.type() == typeid(Integer)) {
        set(owner, std::any_cast<const Integer&>(index.value), value);
    }
}

void Evaluator::Impl::set(UnorderedSet& owner, const Object& index,
                          const Object& value)
{
    owner.value.erase({Setter({{index, value}})});
    owner.value.insert({Setter({{index, value}})});
}

void Evaluator::Impl::set(List& owner, const Integer& index,
                          const Object& value)
{
    if (static_cast<std::size_t>(index.value) < owner.value.size()) {
        auto iter = owner.value.begin();
        std::advance(iter, index.value);
        *iter = value;
    }
}

void Evaluator::Impl::set(List& owner, const Object& index, const Object& value)
{
    if (index.value.type() == typeid(Integer)) {
        set(owner, std::any_cast<const Integer&>(index.value), value);
    }
}

void Evaluator::Impl::set(Object& owner, const Getter& index,
                          const Object& value)
{
    set(*get(owner, index.value.first), eval(index.value.second), value);
}

void Evaluator::Impl::set(Object& owner, const Object& index,
                          const Object& value)
{
    if (index.value.type() == typeid(Getter)) {
        set(owner, std::any_cast<const Getter&>(index.value), value);
    }
    if (owner.value.type() == typeid(UnorderedSet)) {
        set(std::any_cast<UnorderedSet&>(owner.value), index, value);
    }
    if (owner.value.type() == typeid(Vector)) {
        set(std::any_cast<Vector&>(owner.value), index, value);
    }
    if (owner.value.type() == typeid(List)) {
        set(std::any_cast<List&>(owner.value), index, value);
    }
}

Object Evaluator::Impl::cond_(const List& list)
{
    auto iter = list.value.begin();
    while (++iter != list.value.end()) {
        Object result = eval(*iter);
        ++iter;
        if (result.value.type() != typeid(Null)
            && result != Object({Boolean({false})})) {
            return eval(*iter);
        }
    }
    return {Null()};
}

Object Evaluator::Impl::eval_(const List& list)
{
    if (list.value.size() < 2) {
        return {Null()};
    }
    return eval(*++list.value.begin());
}

Object Evaluator::Impl::global_()
{
    return global_scope_;
}

Object Evaluator::Impl::if_(const List& list)
{
    auto iter = ++list.value.begin();
    Object result = eval(*iter);
    if (result.value.type() == typeid(Null)
        || result == Object({Boolean({false})})) {
        if (++iter != list.value.end() && ++iter != list.value.end()) {
            return eval(*iter);
        }
        return {Null()};
    }
    if (++iter != list.value.end()) {
        return eval(*iter);
    }
    return {Null()};
}

Object Evaluator::Impl::import_(const List& list)
{
    auto iter = ++list.value.begin();
    if (iter == list.value.end()) {
        return {Null()};
    }
    PrimitiveFunction function;
    if (iter->value.type() != typeid(String)) {
        return {Null()};
    }
    std::filesystem::path path =
        std::any_cast<const String&>(iter->value).value;
    if (std::filesystem::exists(path) == false) {
        return {Null()};
    }
    if (++iter == list.value.end()) {
        std::fstream file(path.string());
        Object to_eval;
        Reader read;
        while (file) {
            read(to_eval, file);
            eval(to_eval);
        }
        return global_();
    }
    if (iter->value.type() != typeid(String)) {
        return {Null()};
    }
    std::string name = std::any_cast<const String&>(iter->value).value;
    boost::dll::shared_library lib(path.string());
    if (lib.has(name) == false) {
        return {Null()};
    }
    boost::shared_ptr<Object> obj_ptr =
        boost::dll::import<Object>(path.string(), name);
    SharedObject shared_object = {std::shared_ptr<Object>(
        obj_ptr.get(), [obj_ptr](Object*) mutable { obj_ptr.reset(); })};
    Object first = {Symbol({name})};
    Object second = {shared_object};
    set(global_scope_, first, second);
    return eval(shared_object);
}

Object Evaluator::Impl::let_(const List& list)
{
    if (list.value.size() < 3) {
        return {Null()};
    }
    auto iter = ++list.value.begin();
    Setter setter({{*iter, *++iter}});
    Object after_eval = eval(setter.value.second);
    set(local_scope_, setter.value.first, after_eval);
    return after_eval;
}

Object Evaluator::Impl::local_()
{
    return local_scope_;
}

Object Evaluator::Impl::locate_(const List& list)
{
    auto iter = ++list.value.begin();
    if (iter == list.value.end()) {
        return {String({std::filesystem::current_path().string()})};
    }
    boost::dll::fs::path path = boost::dll::program_location().parent_path();
    if (*iter == Object({Symbol({"bin"})})) {
        return {String({path.string()})};
    }
    path = path.parent_path();
    if (*iter == Object({Symbol({"fundot"})})) {
        return {String({path.string()})};
    }
    if (*iter == Object({Symbol({"lib"})})) {
        return {String({(path / "lib").string()})};
    }
    return {Null()};
}

Object Evaluator::Impl::while_(const List& list)
{
    auto iter = ++list.value.begin();
    Object predicate = *iter;
    Object to_eval = *++iter;
    Object result = eval(predicate);
    Object ret({Null()});
    while (result.value.type() != typeid(Null)
           && result != Object({Boolean({false})})) {
        ret = eval(to_eval);
        result = eval(predicate);
    }
    return ret;
}

Object Evaluator::Impl::eval(const Adder& adder)
{
    return eval(adder.value.first) + eval(adder.value.second);
}

Object Evaluator::Impl::eval(const Subtractor& subtractor)
{
    return eval(subtractor.value.first) - eval(subtractor.value.second);
}

Object Evaluator::Impl::eval(const Multiplier& multiplier)
{
    return eval(multiplier.value.first) * eval(multiplier.value.second);
}

Object Evaluator::Impl::eval(const Divisor& divisor)
{
    return eval(divisor.value.first) / eval(divisor.value.second);
}

Object Evaluator::Impl::eval(const Modular& modular)
{
    return eval(modular.value.first) % eval(modular.value.second);
}

Object Evaluator::Impl::eval(const Negator& negator)
{
    return -eval(negator.value);
}

Object Evaluator::Impl::eval(const Less& less)
{
    return {Boolean({eval(less.value.first) < eval(less.value.second)})};
}

Object Evaluator::Impl::eval(const Greater& greater)
{
    return {Boolean({eval(greater.value.first) > eval(greater.value.second)})};
}

Object Evaluator::Impl::eval(const LessEqual& less_equal)
{
    return {Boolean(
        {eval(less_equal.value.first) <= eval(less_equal.value.second)})};
}

Object Evaluator::Impl::eval(const GreaterEqual& greater_equal)
{
    return {Boolean(
        {eval(greater_equal.value.first) >= eval(greater_equal.value.second)})};
}

Object Evaluator::Impl::eval(const EqualTo& equal_to)
{
    return {
        Boolean({eval(equal_to.value.first) == eval(equal_to.value.second)})};
}

Object Evaluator::Impl::eval(const NotEqualTo& not_equal_to)
{
    return {Boolean(
        {eval(not_equal_to.value.first) != eval(not_equal_to.value.second)})};
}

Object Evaluator::Impl::eval(const And& logical_and)
{
    return eval(logical_and.value.first) && eval(logical_and.value.second);
}

Object Evaluator::Impl::eval(const Or& logical_or)
{
    return eval(logical_or.value.first) || eval(logical_or.value.second);
}

Object Evaluator::Impl::eval(const Not& logical_not)
{
    return !eval(logical_not.value);
}

Object Evaluator::Impl::eval(const BitwiseAnd& bitwise_and)
{
    return eval(bitwise_and.value.first) & eval(bitwise_and.value.second);
}

Object Evaluator::Impl::eval(const BitwiseOr& bitwise_or)
{
    return eval(bitwise_or.value.first) | eval(bitwise_or.value.second);
}

Object Evaluator::Impl::eval(const BitwiseXor& bitwise_xor)
{
    return eval(bitwise_xor.value.first) ^ eval(bitwise_xor.value.second);
}

Object Evaluator::Impl::eval(const BitwiseNot& bitwise_not)
{
    return ~eval(bitwise_not.value);
}

Object Evaluator::Impl::eval(const LeftShift& left_shift)
{
    return eval(left_shift.value.first) << eval(left_shift.value.second);
}

Object Evaluator::Impl::eval(const RightShift& right_shift)
{
    return eval(right_shift.value.first) >> eval(right_shift.value.second);
}

Object Evaluator::Impl::eval(const Symbol& symbol)
{
    Object* obj_ptr = get(local_scope_, {symbol});
    if (obj_ptr != nullptr) {
        return *obj_ptr;
    }
    obj_ptr = get(global_scope_, {symbol});
    if (obj_ptr != nullptr) {
        return *obj_ptr;
    }
    return {symbol};
}

Object Evaluator::Impl::eval(const Getter& getter)
{
    Object* obj_ptr = get(local_scope_, {getter});
    if (obj_ptr != nullptr) {
        return *obj_ptr;
    }
    obj_ptr = get(global_scope_, {getter});
    if (obj_ptr != nullptr) {
        return *obj_ptr;
    }
    return {Null()};
}

Object Evaluator::Impl::eval(const Setter& setter)
{
    Object after_eval = eval(setter.value.second);
    set(global_scope_, setter.value.first, after_eval);
    return after_eval;
}

Object Evaluator::Impl::eval(const Assignment& assignment)
{
    return eval(
        {Setter({{eval(assignment.value.first), assignment.value.second}})});
}

Object Evaluator::Impl::eval(const List& list)
{
    if (list.value.empty()) {
        return {Null()};
    }
    List after_eval = list;
    auto iter = after_eval.value.begin();
    *iter = eval(*iter);
    const Object& front = after_eval.value.front();
    if (front.value.type() == typeid(SpecialForm)) {
        const SpecialForm& special_form =
            std::any_cast<const SpecialForm&>(front.value);
        return special_form.value(list);
    }
    while (++iter != after_eval.value.end()) {
        *iter = eval(*iter);
    }
    if (front.value.type() == typeid(PrimitiveFunction)) {
        const PrimitiveFunction& function =
            std::any_cast<const PrimitiveFunction&>(front.value);
        return function.value(after_eval);
    }
    Object ret;
    if (after_eval.value.size() == 1) {
        ret = after_eval.value.back();
    }
    else {
        ret = {after_eval};
    }
    if (front.value.type() == typeid(UnorderedSet)) {
        auto set = std::any_cast<const UnorderedSet&>(front.value);
        Object* type = get(set, {Symbol({"type"})});
        if (type == nullptr) {
            return ret;
        }
        if (*type == Object({Symbol({"function"})})) {
            Object* params = get(set, {Symbol({"params"})});
            if (params == nullptr || params->value.type() != typeid(Vector)) {
                return ret;
            }
            Object* body = get(set, {Symbol({"body"})});
            if (body == nullptr) {
                return ret;
            }
            Object local = local_scope_;
            auto vector = std::any_cast<const Vector&>(params->value).value;
            auto it = after_eval.value.begin();
            for (const auto& arg : vector) {
                if (++it == after_eval.value.end()) {
                    break;
                }
                List let;
                let.value.push_back({Symbol({"let"})});
                let.value.push_back(arg);
                let.value.push_back(*it);
                eval(let);
            }
            Object after = eval(*body);
            local_scope_ = local;
            return after;
        }
    }
    return ret;
}

Object Evaluator::Impl::eval(const Quote& quote)
{
    return quote.value;
}

Object Evaluator::Impl::eval(const SharedObject& shared_object)
{
    return eval(*shared_object.value);
}

Object Evaluator::Impl::eval(const Object& object)
{
    if (object.value.type() == typeid(Adder)) {
        return eval(std::any_cast<const Adder&>(object.value));
    }
    if (object.value.type() == typeid(Subtractor)) {
        return eval(std::any_cast<const Subtractor&>(object.value));
    }
    if (object.value.type() == typeid(Multiplier)) {
        return eval(std::any_cast<const Multiplier&>(object.value));
    }
    if (object.value.type() == typeid(Divisor)) {
        return eval(std::any_cast<const Divisor&>(object.value));
    }
    if (object.value.type() == typeid(Modular)) {
        return eval(std::any_cast<const Modular&>(object.value));
    }
    if (object.value.type() == typeid(Negator)) {
        return eval(std::any_cast<const Negator&>(object.value));
    }
    if (object.value.type() == typeid(Less)) {
        return eval(std::any_cast<const Less&>(object.value));
    }
    if (object.value.type() == typeid(Greater)) {
        return eval(std::any_cast<const Greater&>(object.value));
    }
    if (object.value.type() == typeid(LessEqual)) {
        return eval(std::any_cast<const LessEqual&>(object.value));
    }
    if (object.value.type() == typeid(GreaterEqual)) {
        return eval(std::any_cast<const GreaterEqual&>(object.value));
    }
    if (object.value.type() == typeid(EqualTo)) {
        return eval(std::any_cast<const EqualTo&>(object.value));
    }
    if (object.value.type() == typeid(NotEqualTo)) {
        return eval(std::any_cast<const NotEqualTo&>(object.value));
    }
    if (object.value.type() == typeid(And)) {
        return eval(std::any_cast<const And&>(object.value));
    }
    if (object.value.type() == typeid(Or)) {
        return eval(std::any_cast<const Or&>(object.value));
    }
    if (object.value.type() == typeid(Not)) {
        return eval(std::any_cast<const Not&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseAnd)) {
        return eval(std::any_cast<const BitwiseAnd&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseOr)) {
        return eval(std::any_cast<const BitwiseOr&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseXor)) {
        return eval(std::any_cast<const BitwiseXor&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseNot)) {
        return eval(std::any_cast<const BitwiseNot&>(object.value));
    }
    if (object.value.type() == typeid(LeftShift)) {
        return eval(std::any_cast<const LeftShift&>(object.value));
    }
    if (object.value.type() == typeid(RightShift)) {
        return eval(std::any_cast<const RightShift&>(object.value));
    }
    if (object.value.type() == typeid(Symbol)) {
        return eval(std::any_cast<const Symbol&>(object.value));
    }
    if (object.value.type() == typeid(Getter)) {
        return eval(std::any_cast<const Getter&>(object.value));
    }
    if (object.value.type() == typeid(Setter)) {
        return eval(std::any_cast<const Setter&>(object.value));
    }
    if (object.value.type() == typeid(Assignment)) {
        return eval(std::any_cast<const Assignment&>(object.value));
    }
    if (object.value.type() == typeid(List)) {
        return eval(std::any_cast<const List&>(object.value));
    }
    if (object.value.type() == typeid(Quote)) {
        return eval(std::any_cast<const Quote&>(object.value));
    }
    if (object.value.type() == typeid(SharedObject)) {
        return eval(std::any_cast<const SharedObject&>(object.value));
    }
    return object;
}

Object Evaluator::Impl::eval(const std::string& script)
{
    std::stringstream script_stream(script);
    Object to_eval;
    Reader read;
    read(to_eval, script_stream);
    return eval(to_eval);
}

Object Evaluator::operator()(const Object& object)
{
    return pimpl_->eval(object);
}

Evaluator::Evaluator() : pimpl_(new Evaluator::Impl)
{
    Symbol name;
    PrimitiveFunction function;
    name.value = "eval";
    function.value = [this](const List& list) {
        return pimpl_->eval_(list);
    };
    pimpl_->eval(Setter({{{name}, {function}}}));
    name.value = "global";
    function.value = [this](const List&) {
        return pimpl_->global_();
    };
    pimpl_->eval(Setter({{{name}, {function}}}));
    name.value = "import";
    function.value = [this](const List& list) {
        return pimpl_->import_(list);
    };
    pimpl_->eval(Setter({{{name}, {function}}}));
    name.value = "local";
    function.value = [this](const List&) {
        return pimpl_->local_();
    };
    pimpl_->eval(Setter({{{name}, {function}}}));
    name.value = "locate";
    function.value = [this](const List& list) {
        return pimpl_->locate_(list);
    };
    pimpl_->eval(Setter({{{name}, {function}}}));
    SpecialForm special_from;
    name.value = "cond";
    special_from.value = [this](const List& list) {
        return pimpl_->cond_(list);
    };
    pimpl_->eval(Setter({{{name}, {special_from}}}));
    name.value = "if";
    special_from.value = [this](const List& list) {
        return pimpl_->if_(list);
    };
    pimpl_->eval(Setter({{{name}, {special_from}}}));
    name.value = "let";
    special_from.value = [this](const List& list) {
        return pimpl_->let_(list);
    };
    pimpl_->eval(Setter({{{name}, {special_from}}}));
    name.value = "while";
    special_from.value = [this](const List& list) {
        return pimpl_->while_(list);
    };
    pimpl_->eval(Setter({{{name}, {special_from}}}));
    boost::dll::fs::path path =
        boost::dll::program_location().parent_path().parent_path();
    if (std::filesystem::exists((path / "init.fun").string())) {
        pimpl_->eval("(import \"" + (path / "init.fun").string() + "\")");
    }
}

Evaluator::~Evaluator() = default;

}  // namespace fundot
