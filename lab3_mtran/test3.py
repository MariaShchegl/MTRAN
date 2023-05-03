def likes(names, gb):
    if not names:
        return 'no one likes this'
    else: 
        if len(names) == 1:
           return f'{names[0]} likes this'
        elif len(names)==2:
            return f'{names[0]} and {names[1]} like this'
        elif len(names) == 3:
            return f'{names[0]}, {names[1]} and {names[2]} like this'
        else:
            others = len(names) - 2
            return f'{names[0]}, {names[1]} and {others} others like this'

print(likes([]))
print(likes(["Peter"]))
print(likes(["Jacob", "Alex"]))
print(likes(["Max", "John", "Mark"]))
print(likes(["Alex", "Jacob", "Mark", "Max"]))