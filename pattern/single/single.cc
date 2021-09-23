#include <string.h>
#include <assert.h>
class object
{
public:
    static class object* pObject;
    static object* create_new_object()
    {
        if(NULL != pObject)
			return pObject;
 
		pObject = new object();
		assert(NULL != pObject);
		return pObject;
    }
	
private:
    object() {}
    ~object() {}
};
class object* object::pObject = NULL;

int main(int argc, char* argv[])
{
	object* pGlobal = object::create_new_object();
	return 1;
}