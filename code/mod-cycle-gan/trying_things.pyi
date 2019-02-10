#%%
import tensorflow as tf

from cycle import utils

sess = tf.InteractiveSession()

def make_sample(x):
    return tf.ones((1, x, x), dtype=x.dtype) * x

#%%
# some demo
dataset = tf.data.Dataset.from_tensor_slices(tf.range(1, 30))
dataset = dataset.map(make_sample)
feed = dataset.make_one_shot_iterator().get_next()

memory = tf.TensorArray(dtype=tf.int32, size=50, dynamic_size=True, infer_shape=False)

# memory = memory.write(0, [[4, 5]])
memory = memory.write(1, [[1]])
memory = memory.write(2, -3)

memory = memory.write(0, feed)
# r0 = memory.read(0)
# r1 = memory.read(1)
# r2 = memory.read(2)
mem_tensor = memory.stack()

#%%
# vals = sess.run([feed, memory.stack()])
vals = sess.run({
    'feed': feed,
    # 'r0': r0,
    # 'r1': r1,
    # 'r2': r2,
    'mem_tensor': memory.size()
})

#%%
# simulating whole DatBuffer behaviour
# dataset = tf.data.Dataset.from_tensor_slices(tf.range(1, 30))
# dataset = dataset.map(make_sample)
# feed = dataset.make_one_shot_iterator().get_next()

# memory = tf.TensorArray(dtype=tf.int32, size=50, dynamic_size=True, infer_shape=False)
# memory_free_idx = tf.get_variable(dtype=tf.int32, initializer=tf.zeros_initializer())
# pool_capacity = memory.size()

# def add_new_element():
#     memory.write(memory_free_idx, feed)
#     increment = tf.assign_add(memory_free_idx, 1)
#     pass
#
# def overwrite():
#     pass

# tf.cond(memory_free_idx > pool_capacity, overwrite, add_new_element)
# memory = memory.write(0, feed)
# mem_tensor = memory.stack()

#%%
# simulating whole DataBuffer behaviour with py_func
dataset = tf.data.Dataset.from_tensor_slices(tf.range(1, 30))
dataset = dataset.map(make_sample)
feed = dataset.make_one_shot_iterator().get_next()
generator = feed * 2

global_step = tf.get_variable('global_step', dtype=tf.int32, initializer=tf.zeros_initializer(), shape=())
sess.run(tf.global_variables_initializer())
increment_global_step_op = tf.assign(global_step, global_step+1)

x_pool = utils.DataBuffer(pool_size=10, batch_size=1)

def queue(fake_img, step):
    return x_pool.query(fake_img, step)

pooled_data = tf.py_func(queue, [generator, tf.train.get_or_create_global_step()], tf.int32)

#%%
values = sess.run({
    'feed': feed,
    'generator': generator,
    'pooled_data': pooled_data,
    'op': increment_global_step_op,
})
