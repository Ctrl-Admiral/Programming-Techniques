# Google C++ Style Guide
---

### Table of contents

- [Background](#background)
    - [Goals of the Style Guide](#goals-of-the-style-guide)
- [C++ Version](#c-version)
- [Header Files](#header-files)
    - [Self-contained Headers](#self-contained-headers)
    - [The #define Guard](#the-define-guard)
    - [Inline Functions](#inline-functions)
    - [Names and Order of Includes](#names-and-order-of-includes)
- [Scoping](#scoping)
    - [Namespaces](#namespaces)
    - [Unnamed Namespaces and Static Variables](#unnamed-namespaces-and-static-variables)
    - [Nonmember, Static Member, and Global Functions](#nonmember-static-member-and-global-functions)
    - [Local variables](#local-variables)
    - [Static and Global Variables](#static-and-global-variables)

---
## Background
C++ is one of the main development languages used by many of Google's open-source projects. As every C++ programmer knows, the language has many powerful features, but this power brings with it complexity, which in turn can make code more bug-prone and harder to read and maintain.

The goal of this guide is to manage this complexity by describing in detail the dos and don'ts of writing C++ code . These rules exist to keep the code base manageable while still allowing coders to use C++ language features productively.

*Style*, also known as readability, is what we call the conventions that govern our C++ code. The term Style is a bit of a misnomer, since these conventions cover far more than just source file formatting.

Most open-source projects developed by Google conform to the requirements in this guide.

Note that this guide is not a C++ tutorial: we assume that the reader is familiar with the language.

### Goals of the Style Guide
Why do we have this document?

There are a few core goals that we believe this guide should serve. These are the fundamental whys that underlie all of the individual rules. By bringing these ideas to the fore, we hope to ground discussions and make it clearer to our broader community why the rules are in place and why particular decisions have been made. If you understand what goals each rule is serving, it should be clearer to everyone when a rule may be waived (some can be), and what sort of argument or alternative would be necessary to change a rule in the guide.

The goals of the style guide as we currently see them are as follows:

***Style rules should pull their weight.***
The benefit of a style rule must be large enough to justify asking all of our engineers to remember it. The benefit is measured relative to the codebase we would get without the rule, so a rule against a very harmful practice may still have a small benefit if people are unlikely to do it anyway. This principle mostly explains the rules we don’t have, rather than the rules we do: for example, goto contravenes many of the following principles, but is already vanishingly rare, so the Style Guide doesn’t discuss it.

***Optimize for the reader, not the writer.***
Our codebase (and most individual components submitted to it) is expected to continue for quite some time. As a result, more time will be spent reading most of our code than writing it. We explicitly choose to optimize for the experience of our average software engineer reading, maintaining, and debugging code in our codebase rather than ease when writing said code. "Leave a trace for the reader" is a particularly common sub-point of this principle: When something surprising or unusual is happening in a snippet of code (for example, transfer of pointer ownership), leaving textual hints for the reader at the point of use is valuable (std::unique_ptr demonstrates the ownership transfer unambiguously at the call site).

***Be consistent with existing code.***
Using one style consistently through our codebase lets us focus on other (more important) issues. Consistency also allows for automation: tools that format your code or adjust your #includes only work properly when your code is consistent with the expectations of the tooling. In many cases, rules that are attributed to "Be Consistent" boil down to "Just pick one and stop worrying about it"; the potential value of allowing flexibility on these points is outweighed by the cost of having people argue over them.

***Be consistent with the broader C++ community when appropriate.***
Consistency with the way other organizations use C++ has value for the same reasons as consistency within our code base. If a feature in the C++ standard solves a problem, or if some idiom is widely known and accepted, that's an argument for using it. However, sometimes standard features and idioms are flawed, or were just designed without our codebase's needs in mind. In those cases (as described below) it's appropriate to constrain or ban standard features. In some cases we prefer a homegrown or third-party library over a library defined in the C++ Standard, either out of perceived superiority or insufficient value to transition the codebase to the standard interface.

***Avoid surprising or dangerous constructs.***
C++ has features that are more surprising or dangerous than one might think at a glance. Some style guide restrictions are in place to prevent falling into these pitfalls. There is a high bar for style guide waivers on such restrictions, because waiving such rules often directly risks compromising program correctness.

***Avoid constructs that our average C++ programmer would find tricky or hard to maintain.***
C++ has features that may not be generally appropriate because of the complexity they introduce to the code. In widely used code, it may be more acceptable to use trickier language constructs, because any benefits of more complex implementation are multiplied widely by usage, and the cost in understanding the complexity does not need to be paid again when working with new portions of the codebase. When in doubt, waivers to rules of this type can be sought by asking your project leads. This is specifically important for our codebase because code ownership and team membership changes over time: even if everyone that works with some piece of code currently understands it, such understanding is not guaranteed to hold a few years from now.

***Be mindful of our scale.***
With a codebase of 100+ million lines and thousands of engineers, some mistakes and simplifications for one engineer can become costly for many. For instance it's particularly important to avoid polluting the global namespace: name collisions across a codebase of hundreds of millions of lines are difficult to work with and hard to avoid if everyone puts things into the global namespace.

***Concede to optimization when necessary.***
Performance optimizations can sometimes be necessary and appropriate, even when they conflict with the other principles of this document.

The intent of this document is to provide maximal guidance with reasonable restriction. As always, common sense and good taste should prevail. By this we specifically refer to the established conventions of the entire Google C++ community, not just your personal preferences or those of your team. Be skeptical about and reluctant to use clever or unusual constructs: the absence of a prohibition is not the same as a license to proceed. Use your judgment, and if you are unsure, please don't hesitate to ask your project leads to get additional input.

## C++ Version
Currently, code should target C\++17, i.e., should not use C\++2x features. The C++ version targeted by this guide will advance (aggressively) over time.

Do not use [non-standard extensions](#nonstandard-extensions).

Consider portability to other environments before using features from C++14 and C++17 in your project.

## Header Files
In general, every .cc file should have an associated .h file. There are some common exceptions, such as unit tests and small .cc files containing just a main() function.

Correct use of header files can make a huge difference to the readability, size and performance of your code.

The following rules will guide you through the various pitfalls of using header files.

### Self-contained Headers
Header files should be self-contained (compile on their own) and end in `.h`. Non-header files that are meant for inclusion should end in `.inc` and be used sparingly.

All header files should be self-contained. Users and refactoring tools should not have to adhere to special conditions to include the header. Specifically, a header should have [header guards](#the-define-guard) and include all other headers it needs.

Prefer placing the definitions for template and inline functions in the same file as their declarations. The definitions of these constructs must be included into every `.cc` file that uses them, or the program may fail to link in some build configurations. If declarations and definitions are in different files, including the former should transitively include the latter. Do not move these definitions to separately included header files (`-inl.h`); this practice was common in the past, but is no longer allowed.

As an exception, a template that is explicitly instantiated for all relevant sets of template arguments, or that is a private implementation detail of a class, is allowed to be defined in the one and only `.cc` file that instantiates the template.

There are rare cases where a file designed to be included is not self-contained. These are typically intended to be included at unusual locations, such as the middle of another file. They might not use [header guards](#the-define-guard), and might not include their prerequisites. Name such files with the `.inc` extension. Use sparingly, and prefer self-contained headers when possible.

### The #define Guard
All header files should have #define guards to prevent multiple inclusion. The format of the symbol name should be `<PROJECT>_<PATH>_<FILE>_H_`.

To guarantee uniqueness, they should be based on the full path in a project's source tree. For example, the file `foo/src/bar/baz.h` in project foo should have the following guard:

```
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_
...
#endif  // FOO_BAR_BAZ_H_
```
### Forward Declarations
Avoid using forward declarations where possible. Instead, `#include` the headers you need.

A "forward declaration" is a declaration of a class, function, or template without an associated definition.

- Forward declarations can save compile time, as `#include`s force the compiler to open more files and process more input.
- Forward declarations can save on unnecessary recompilation. `#include`s can force your code to be recompiled more often, due to unrelated changes in the header.
- Forward declarations can hide a dependency, allowing user code to skip necessary recompilation when headers change.
- A forward declaration may be broken by subsequent changes to the library. Forward declarations of functions and templates can prevent the header owners from making otherwise-compatible changes to their APIs, such as widening a parameter type, adding a template parameter with a default value, or migrating to a new namespace.
- Forward declaring symbols from namespace `std::` yields undefined behavior.
- It can be difficult to determine whether a forward declaration or a full `#include` is needed. Replacing an `#include` with a forward declaration can silently change the meaning of code:
```
// b.h:
struct B {};
struct D : B {};

// good_user.cc:
#include "b.h"
void f(B*);
void f(void*);
void test(D* x) { f(x); }  // calls f(B*)
```
If the `#include` was replaced with forward decls for B and D, `test()` would call `f(void*)`.
- Forward declaring multiple symbols from a header can be more verbose than simply `#include`ing the header.
- Structuring code to enable forward declarations (e.g., using pointer members instead of object members) can make the code slower and more complex.
- Try to avoid forward declarations of entities defined in another project.
- When using a function declared in a header file, always `#include` that header.
- When using a class template, prefer to `#include` its header file.
Please see [Names and Order of Includes](#names-and-order-of-includes) for rules about when to #include a header.

### Inline Functions
Define functions inline only when they are small, say, 10 lines or fewer.

You can declare functions in a way that allows the compiler to expand them inline rather than calling them through the usual function call mechanism.

Inlining a function can generate more efficient object code, as long as the inlined function is small. Feel free to inline accessors and mutators, and other short, performance-critical functions.

Overuse of inlining can actually make programs slower. Depending on a function's size, inlining it can cause the code size to increase or decrease. Inlining a very small accessor function will usually decrease code size while inlining a very large function can dramatically increase code size. On modern processors smaller code usually runs faster due to better use of the instruction cache.

A decent rule of thumb is to not inline a function if it is more than 10 lines long. Beware of destructors, which are often longer than they appear because of implicit member- and base-destructor calls!

Another useful rule of thumb: it's typically not cost effective to inline functions with loops or switch statements (unless, in the common case, the loop or switch statement is never executed).

It is important to know that functions are not always inlined even if they are declared as such; for example, virtual and recursive functions are not normally inlined. Usually recursive functions should not be inline. The main reason for making a virtual function inline is to place its definition in the class, either for convenience or to document its behavior, e.g., for accessors and mutators.

### Names and Order of Includes

Include headers in the following order: Related header, C system headers, C++ standard library headers, other libraries' headers, your project's headers.

All of a project's header files should be listed as descendants of the project's source directory without use of UNIX directory aliases . (the current directory) or .. (the parent directory). For example, `google-awesome-project/src/base/logging.h` should be included as: `#include "base/logging.h"`

In `dir/foo.cc` or `dir/foo_test.cc`, whose main purpose is to implement or test the stuff in dir2/foo2.h, order your includes as follows:

1. `dir2/foo2.h`.
2. A blank line
3. C system headers (more precisely: headers in angle brackets with the `.h` extension), e.g., `<unistd.h>`, `<stdlib.h>`.
4. A blank line
5. C++ standard library headers (without file extension), e.g., `<algorithm>`, `<cstddef>`.
6. A blank line
7. Other libraries' `.h` files.
8. Your project's `.h` files.

Separate each non-empty group with one blank line.

With the preferred ordering, if the related header `dir2/foo2.h` omits any necessary includes, the build of `dir/foo.cc` or `dir/foo_test.cc` will break. Thus, this rule ensures that build breaks show up first for the people working on these files, not for innocent people in other packages.

`dir/foo.cc` and `dir2/foo2.h` are usually in the same directory (e.g., `base/basictypes_test.cc` and `base/basictypes.h`), but may sometimes be in different directories too.

Note that the C headers such as stddef.h are essentially interchangeable with their C++ counterparts (cstddef). Either style is acceptable, but prefer consistency with existing code.

Within each section the includes should be ordered alphabetically. Note that older code might not conform to this rule and should be fixed when convenient.

You should include all the headers that define the symbols you rely upon, except in the unusual case of forward declaration. If you rely on symbols from `bar.h`, don't count on the fact that you included foo.h which (currently) includes `bar.h`: include bar.h yourself, unless `foo.h` explicitly demonstrates its intent to provide you the symbols of `bar.h`.

For example, the includes in `google-awesome-project/src/foo/internal/fooserver.cc` might look like this:
```
#include "foo/server/fooserver.h"

#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/commandlineflags.h"
#include "foo/server/bar.h"
```

#### Exception:

Sometimes, system-specific code needs conditional includes. Such code can put conditional includes after other includes. Of course, keep your system-specific code small and localized. Example:
```
#include "foo/public/fooserver.h"

#include "base/port.h"  // For LANG_CXX11.

#ifdef LANG_CXX11
#include <initializer_list>
#endif  // LANG_CXX11
```

## Scoping
### Namespaces

With few exceptions, place code in a namespace. Namespaces should have unique names based on the project name, and possibly its path. Do not use *using-directives* (e.g., `using namespace foo`). Do not use inline namespaces. For unnamed namespaces, see [Unnamed Namespaces and Static Variables](#unnamed-namespaces-and-static-variables).

Namespaces subdivide the global scope into distinct, named scopes, and so are useful for preventing name collisions in the global scope.

Namespaces provide a method for preventing name conflicts in large programs while allowing most code to use reasonably short names.

For example, if two different projects have a class Foo in the global scope, these symbols may collide at compile time or at runtime. If each project places their code in a namespace, project1::Foo and project2::Foo are now distinct symbols that do not collide, and code within each project's namespace can continue to refer to Foo without the prefix.

Inline namespaces automatically place their names in the enclosing scope. Consider the following snippet, for example:

```
namespace outer {
inline namespace inner {
  void foo();
}  // namespace inner
}  // namespace outer
```
The expressions `outer::inner::foo()` and `outer::foo()` are interchangeable. Inline namespaces are primarily intended for ABI compatibility across versions.

Namespaces can be confusing, because they complicate the mechanics of figuring out what definition a name refers to.

Inline namespaces, in particular, can be confusing because names aren't actually restricted to the namespace where they are declared. They are only useful as part of some larger versioning policy.

In some contexts, it's necessary to repeatedly refer to symbols by their fully-qualified names. For deeply-nested namespaces, this can add a lot of clutter.

Namespaces should be used as follows:

- Follow the rules on [Namespace Names](#namespace-names).
- Terminate namespaces with comments as shown in the given examples.
- Namespaces wrap the entire source file after includes, [gflags](https://gflags.github.io/gflags/) definitions/declarations and forward declarations of classes from other namespaces.

```
// In the .h file
namespace mynamespace {

// All declarations are within the namespace scope.
// Notice the lack of indentation.
class MyClass {
 public:
  ...
  void Foo();
};

}  // namespace mynamespace
// In the .cc file
namespace mynamespace {

// Definition of functions is within scope of the namespace.
void MyClass::Foo() {
  ...
}

}  // namespace mynamespace
```
More complex `.cc` files might have additional details, like flags or using-declarations.
```
#include "a.h"

ABSL_FLAG(bool, someflag, false, "dummy flag");

namespace mynamespace {

using ::foo::Bar;

...code for mynamespace...    // Code goes against the left margin.

}  // namespace mynamespace
```

- To place generated protocol message code in a namespace, use the package specifier in the .proto file. See [Protocol Buffer Packages](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#package) for details.
- Do not declare anything in namespace std, including forward declarations of standard library classes. Declaring entities in namespace std is undefined behavior, i.e., not portable. To declare entities from the standard library, include the appropriate header file.
- You may not use a *using-directive* to make all names from a namespace available.
```
// Forbidden -- This pollutes the namespace.
using namespace foo;
```
- Do not use *Namespace aliases* at namespace scope in header files except in explicitly marked internal-only namespaces, because anything imported into a namespace in a header file becomes part of the public API exported by that file.
```
// Shorten access to some commonly used names in .cc files.
namespace baz = ::foo::bar::baz;
// Shorten access to some commonly used names (in a .h file).
namespace librarian {
namespace impl {  // Internal, not part of the API.
namespace sidetable = ::pipeline_diagnostics::sidetable;
}  // namespace impl

inline void my_inline_function() {
  // namespace alias local to a function (or method).
  namespace baz = ::foo::bar::baz;
  ...
}
}  // namespace librarian
```
- Do not use inline namespaces.

### Unnamed Namespaces and Static Variables
When definitions in a `.cc` file do not need to be referenced outside that file, place them in an unnamed namespace or declare them `static`. Do not use either of these constructs in `.h` files.

All declarations can be given internal linkage by placing them in unnamed namespaces. Functions and variables can also be given internal linkage by declaring them `static`. This means that anything you're declaring can't be accessed from another file. If a different file declares something with the same name, then the two entities are completely independent.

Use of internal linkage in `.cc` files is encouraged for all code that does not need to be referenced elsewhere. Do not use internal linkage in `.h` files.

Format unnamed namespaces like named namespaces. In the terminating comment, leave the namespace name empty:
```
namespace {
...
}  // namespace
```

### Nonmember, Static Member, and Global Functions
Prefer placing nonmember functions in a namespace; use completely global functions rarely. Do not use a class simply to group static members. Static methods of a class should generally be closely related to instances of the class or the class's static data.

Nonmember and static member functions can be useful in some situations. Putting nonmember functions in a namespace avoids polluting the global namespace.

Nonmember and static member functions may make more sense as members of a new class, especially if they access external resources or have significant dependencies.

Sometimes it is useful to define a function not bound to a class instance. Such a function can be either a static member or a nonmember function. Nonmember functions should not depend on external variables, and should nearly always exist in a namespace. Do not create classes only to group static members; this is no different than just giving the names a common prefix, and such grouping is usually unnecessary anyway.

If you define a nonmember function and it is only needed in its `.cc` file, use [internal linkage](#unnamed-namespaces-and-static-variables) to limit its scope.

### Local Variables

Place a function's variables in the narrowest scope possible, and initialize variables in the declaration.

C\++ allows you to declare variables anywhere in a function. We encourage you to declare them in as local a scope as possible, and as close to the first use as possible. This makes it easier for the reader to find the declaration and see what type the variable is and what it was initialized to. In particular, initialization should be used instead of declaration and assignment, e.g.,:
```
int i;
i = f();      // Bad -- initialization separate from declaration.
int j = g();  // Good -- declaration has initialization.
std::vector<int> v;
v.push_back(1);  // Prefer initializing using brace initialization.
v.push_back(2);
std::vector<int> v = {1, 2};  // Good -- v starts initialized.
```
Variables needed for if, while and for statements should normally be declared within those statements, so that such variables are confined to those scopes. E.g.:
```
while (const char* p = strchr(str, '/')) str = p + 1;
```
There is one caveat: if the variable is an object, its constructor is invoked every time it enters scope and is created, and its destructor is invoked every time it goes out of scope.
```
// Inefficient implementation:
for (int i = 0; i < 1000000; ++i) {
  Foo f;  // My ctor and dtor get called 1000000 times each.
  f.DoSomething(i);
}
```
It may be more efficient to declare such a variable used in a loop outside that loop:
```
Foo f;  // My ctor and dtor get called once each.
for (int i = 0; i < 1000000; ++i) {
  f.DoSomething(i);
}
```

### Static and Global Variables
Objects with [static storage duration](https://en.cppreference.com/w/cpp/language/storage_duration#Storage_duration) are forbidden unless they are [trivially destructible](https://en.cppreference.com/w/cpp/types/is_destructible). Informally this means that the destructor does not do anything, even taking member and base destructors into account. More formally it means that the type has no user-defined or virtual destructor and that all bases and non-static members are trivially destructible. Static function-local variables may use dynamic initialization. Use of dynamic initialization for static class member variables or variables at namespace scope is discouraged, but allowed in limited circumstances; see below for details.

As a rule of thumb: a global variable satisfies these requirements if its declaration, considered in isolation, could be `constexpr`.

Every object has a *storage duration*, which correlates with its lifetime. Objects with static storage duration live from the point of their initialization until the end of the program. Such objects appear as variables at namespace scope ("global variables"), as static data members of classes, or as function-local variables that are declared with the `static` specifier. Function-local static variables are initialized when control first passes through their declaration; all other objects with static storage duration are initialized as part of program start-up. All objects with static storage duration are destroyed at program exit (which happens before unjoined threads are terminated).

Initialization may be *dynamic*, which means that something non-trivial happens during initialization. (For example, consider a constructor that allocates memory, or a variable that is initialized with the current process ID.) The other kind of initialization is *static* initialization. The two aren't quite opposites, though: static initialization *always* happens to objects with static storage duration (initializing the object either to a given constant or to a representation consisting of all bytes set to zero), whereas dynamic initialization happens after that, if required.

Global and static variables are very useful for a large number of applications: named constants, auxiliary data structures internal to some translation unit, command-line flags, logging, registration mechanisms, background infrastructure, etc.

Global and static variables that use dynamic initialization or have non-trivial destructors create complexity that can easily lead to hard-to-find bugs. Dynamic initialization is not ordered across translation units, and neither is destruction (except that destruction happens in reverse order of initialization). When one initialization refers to another variable with static storage duration, it is possible that this causes an object to be accessed before its lifetime has begun (or after its lifetime has ended). Moreover, when a program starts threads that are not joined at exit, those threads may attempt to access objects after their lifetime has ended if their destructor has already run.

#### Decision on destruction
When destructors are trivial, their execution is not subject to ordering at all (they are effectively not "run"); otherwise we are exposed to the risk of accessing objects after the end of their lifetime. Therefore, we only allow objects with static storage duration if they are trivially destructible. Fundamental types (like pointers and `int`) are trivially destructible, as are arrays of trivially destructible types. Note that variables marked with constexpr are trivially destructible.

```
const int kNum = 10;  // allowed

struct X { int n; };
const X kX[] = {{1}, {2}, {3}};  // allowed

void foo() {
  static const char* const kMessages[] = {"hello", "world"};  // allowed
}

// allowed: constexpr guarantees trivial destructor
constexpr std::array<int, 3> kArray = {{1, 2, 3}};
// bad: non-trivial destructor
const std::string kFoo = "foo";

// bad for the same reason, even though kBar is a reference (the
// rule also applies to lifetime-extended temporary objects)
const std::string& kBar = StrCat("a", "b", "c");

void bar() {
  // bad: non-trivial destructor
  static std::map<int, int> kData = {{1, 0}, {2, 0}, {3, 0}};
}
```
Note that references are not objects, and thus they are not subject to the constraints on destructibility. The constraint on dynamic initialization still applies, though. In particular, a function-local static reference of the form `static T& t = *new T;` is allowed.

#### Decision on initialization
Initialization is a more complex topic. This is because we must not only consider whether class constructors execute, but we must also consider the evaluation of the initializer:
```
int n = 5;    // fine
int m = f();  // ? (depends on f)
Foo x;        // ? (depends on Foo::Foo)
Bar y = g();  // ? (depends on g and on Bar::Bar)
```

All but the first statement expose us to indeterminate initialization ordering.

The concept we are looking for is called *constant initialization* in the formal language of the C++ standard. It means that the initializing expression is a constant expression, and if the object is initialized by a constructor call, then the constructor must be specified as `constexpr`, too:
```
struct Foo { constexpr Foo(int) {} };

int n = 5;  // fine, 5 is a constant expression
Foo x(2);   // fine, 2 is a constant expression and the chosen constructor is constexpr
Foo a[] = { Foo(1), Foo(2), Foo(3) };  // fine
```

Constant initialization is always allowed. Constant initialization of static storage duration variables should be marked with `constexpr` or where possible the [ABSL_CONST_INIT](https://github.com/abseil/abseil-cpp/blob/03c1513538584f4a04d666be5eb469e3979febba/absl/base/attributes.h#L540) attribute. Any non-local static storage duration variable that is not so marked should be presumed to have dynamic initialization, and reviewed very carefully.

By contrast, the following initializations are problematic:
```
// Some declarations used below.
time_t time(time_t*);      // not constexpr!
int f();                   // not constexpr!
struct Bar { Bar() {} };

// Problematic initializations.
time_t m = time(nullptr);  // initializing expression not a constant expression
Foo y(f());                // ditto
Bar b;                     // chosen constructor Bar::Bar() not constexpr
```

Dynamic initialization of nonlocal variables is discouraged, and in general it is forbidden. However, we do permit it if no aspect of the program depends on the sequencing of this initialization with respect to all other initializations. Under those restrictions, the ordering of the initialization does not make an observable difference. For example:
```
int p = getpid();  // allowed, as long as no other static variable
                   // uses p in its own initialization
```
Dynamic initialization of static local variables is allowed (and common).
