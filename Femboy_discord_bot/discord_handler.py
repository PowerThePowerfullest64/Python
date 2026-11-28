# This example requires the 'message_content' intent.
import discord
import asyncio
import Femboy
import re

mention_labels = [
    "zack",
    "catboy",
    "gayboy",
    "femboy",
    "ladyboy",
    "fem",
    "boy",
    "gay",
    "nigger",
    "sack",
    "zuck",
    "zucker",
    "zacky",
    "cat",
    "kitten",
    "kitty",
    "miaw",
    "meow",
    "neger",
    "nigga",
    "nga",
    "zawg",
    "dawg",
]

presence = "Feelin' extra fuzzy today! 😽✨"

async def do_commands(reply: str, message: discord.Message):
    match = re.search(r"\[KICK:\s*([^,\]]+?)\s*,\s*(.*?)\]", reply, re.IGNORECASE)
    if not match:
        return reply

    target_name = match.group(1).strip()
    reason = match.group(2).strip() or "No reason provided"
    clean_reply = re.sub(r"\[KICK:.*?\]", "", reply, flags=re.IGNORECASE).strip()

    # Message was typed in a dm.
    if not message.guild:
        print(f"Warning: Tried to kick someone of name '{target_name}' while in a dm.")
        return clean_reply

    target_member = message.guild.get_member_named(target_name)

    if target_member is None:
        # Fallback to check if femboy used id instead of name.
        id_match = re.search(r"\d{15,20}", target_name)
        if id_match:
            user_id = int(id_match.group(0))
            target_member = message.guild.get_member(user_id)
    if target_member is None:
        # Fallback to check if femboy typed case wrong.
        target_member = message.guild.get_member_named(target_name.lower())

    if target_member is None:
        print(f"Warning: Tried to kick someone that didn't exist of name '{target_name}'.")
        return clean_reply
    if target_member == message.guild.me:
        print("Warning: Tried to kick self.")
        return clean_reply

    try:
        await target_member.kick(reason=reason)
    except discord.Forbidden:
        print(f"Warning: Tried to kick someone with a higher permission of name '{target_member.name}'.")
    return clean_reply
        
with open("token.txt", "r") as t:
    TOKEN = t.read()

intents = discord.Intents.default()
intents.message_content = True
#intents.members = True # Andreas needs to fix this!
client = discord.Client(intents=intents)

@client.event
async def on_ready():
    print(f'{client.user} is connected to the following server:\n')
    for server in client.guilds:
        print(f'{server.name} (id: {server.id})')

    await client.change_presence(
        activity=discord.Activity(type=discord.ActivityType.listening, name=presence)
    )

@client.event
async def on_message(message: discord.Message):
    if message.author == client.user:
        return

    if message.author.bot: # Prevent bot back-and-fourth.
        return
    
    clean_content = message.content
    is_mentioned = False
    
    for label in mention_labels:
        if label.lower() in message.content.lower():
            is_mentioned = True
            break

    if client.user in message.mentions:
        is_mentioned = True
        clean_content = (
            clean_content.replace(f"<@{client.user.id}>", "")
            .replace(f"<@!{client.user.id}>", "")
        )

    if clean_content.startswith("!zack"):
        clean_content = clean_content.removeprefix("!zack")
        is_mentioned = True

    clean_content = clean_content.strip()

    if not is_mentioned:
        # Save message to history even though zack wasn't mentioned.
        Femboy.save_user_history(clean_content, message.author.name)
        return
    
    async with message.channel.typing():
        if clean_content == "history":
            history = "This is the chat history that I have recorded, (*nya*):\n"
            for i, msg in enumerate(Femboy.messages):
                if i == 0: # Skip system prompt.
                    continue
                history += msg["content"] + "\n"
            await message.channel.send(history)
            return

        print("-----------------------------------------------------------")
        print(clean_content)

        if message.reference and message.reference.message_id:
            try:
                replied_msg = await message.channel.fetch_message(message.reference.message_id)
                clean_content = f"[Replying to {replied_msg.author.name}: \"{replied_msg.content}\"]\n{clean_content}"
            except discord.NotFound:
                pass
        
        raw_reply = await asyncio.to_thread(Femboy.do_chat, clean_content, message.author.name)
        print(raw_reply)
        clean_reply = await do_commands(raw_reply, message)
        clean_reply = clean_reply[:2000] # Cut off any characters outside 2000 char limit.
        if clean_reply:
            await message.reply(clean_reply)
    

client.run(TOKEN)