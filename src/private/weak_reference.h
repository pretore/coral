#ifndef _CORAL_PRIVATE_WEAK_REFERENCE_H_
#define _CORAL_PRIVATE_WEAK_REFERENCE_H_

#include <stddef.h>
#include <stdbool.h>

#include "object.h"

#define CORAL_CLASS_LOAD_PRIORITY_WEAK_REFERENCE \
    (1 + CORAL_CLASS_LOAD_PRIORITY_OBJECT)

struct coral$weak_reference {
    void *object;
};

/**
 * @brief Initialize the weak reference instance.
 * @param [in] object weak reference to be initialized.
 * @param [in] instance for which we will be a weak reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance is <i>NULL</i>.
 */
bool coral$weak_reference$init(struct coral$weak_reference *object,
                               void *instance);

/**
 * @brief Initialize the weak reference instance.
 * @param [in] object weak reference to be initialized.
 * @param [in] instance for which we will be a weak reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance is <i>NULL</i>.
 */
bool coral$weak_reference$invalidate(struct coral$weak_reference *object);

/**
 * @brief Retrieve the <i>void*</i> address.
 * @param [in] object weak reference instance.
 * @param [out] out receive instance for which we are a weak reference for.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if there is no instance contained in
 * the weak reference.
 */
bool coral$weak_reference$get(struct coral$weak_reference *object, void **out);

#endif /* _CORAL_PRIVATE_WEAK_REFERENCE_H_ */