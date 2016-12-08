#ifndef __SERVICECALL__
#define __SERVICECALL__

template < typename TClass, typename reType = void >struct serviceCall
{
	 reType (TClass::*fpt) (void *);	//!< pointer to member function
	 TClass *pt2Object;						//!< pointer to object

	   serviceCall ()
	 {
			pt2Object = 0;
			fpt = 0;
	 }

	 serviceCall (TClass * _pt2Object, reType (TClass::*_fpt) (void *))
	 {
			setServiceCall (_pt2Object, _fpt);
	 }

	 void setServiceCall (TClass * _pt2Object, reType (TClass::*_fpt) (void *))
	 {
			pt2Object = _pt2Object;
			fpt = _fpt;
	 }

	 reType operator  () (void *data = 0)
	 {
			return (*pt2Object.*fpt) (data);
	 }
};

#endif
