#ifndef _CORAL_PRIVATE_REFERENCE_H_
#define _CORAL_PRIVATE_REFERENCE_H_

#include <stddef.h>
#include <stdbool.h>

#include "object.h"

#define CORAL_CLASS_LOAD_PRIORITY_REFERENCE \
    (1 + CORAL_CLASS_LOAD_PRIORITY_OBJECT)

struct coral$reference {
    void *object;
};

/**
 * @brief Initialize the reference instance.
 * @param [in] object reference to be initialized.
 * @param [in] instance for which we will be a strong reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance is <i>NULL</i>.
 */
bool coral$reference$init(struct coral$reference *object, void *instance);

/**
 * @brief Invalidate the reference.
 * <p>The actual <u>reference is not deallocated</u> since it may have been
 * embedded in a larger structure.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$reference$invalidate(struct coral$reference *object);

/**
 * @brief Retrieve the <i>void*</i> address.
 * @param [in] object reference instance.
 * @param [out] out receive instance for which we are a strong reference for.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$reference$get(struct coral$reference *object, void **out);

#endif /* _CORAL_PRIVATE_REFERENCE_H_ */